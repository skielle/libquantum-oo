/*
 * register.cpp
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <vector>
#include "complex.h"
#include "config.h"
#include "defs.h"
#include "error.h"
#include "matrix.h"
#include "node.h"
#include "register.h"

using namespace std;

namespace Quantum {
Register::Register() {
	Error::error(QUANTUM_EOPCODE);
}

Register::Register(Matrix *m, int width) {
	int i, j;
	int size = 0;
	
	srand(time(NULL));

	if ( m->getCols() != 1 ) {
		Error::error(QUANTUM_EMCMATRIX);
	}

	this->width = width;

	for ( i = 0; i < m->getRows(); i++ ) {
		if ( m->get(i, 0) ) {
			size++;
		}
	}

	this->size = size;
	this->hashw = width + 2;

	this->node.reserve(size);

	if ( !this->node.max_size() == size ) {
		Error::error(QUANTUM_ENOMEM);
	}

	//memman stuff

	this->hash = new int[1 << this->hashw]; 

	if ( !this->hash ) {
		Error::error(QUANTUM_ENOMEM);
	}

	//memman stuff

	for ( i = 0, j = 0; i < m->getRows(); i++ ) {
		if ( m->get(i, 0) ) {
			Node* n = new Node( m->get(i, 0), i );
			this->node.push_back(n);
			j++;
		}
	}
}
	
Register::Register(MAX_UNSIGNED initval, int width) {
	srand(time(NULL));

	this->width = width;
	this->size = 1;
	this->hashw = width + 2;

	this->node.reserve(size);

	if ( !this->node.max_size() == size ) {
		Error::error(QUANTUM_ENOMEM);
	}

	//memman stuff

	this->hash = new int[1 << this->hashw]; 

	if ( !this->hash ) {
		Error::error(QUANTUM_ENOMEM);
	}

	//memman stuff
	
	Node* n = new Node(1, initval);
	this->node.push_back(n);
}

Register::Register(int n, int width) {
	this->width = width;
	this->size = n;
	this->hashw = 0;
	this->hash = 0;

	this->node.reserve(size);
	
	srand(time(NULL));

	if ( !this->node.max_size() == size ) {
		Error::error(QUANTUM_ENOMEM);
	}

	//memman stuff
}

int Register::getWidth() {
	return this->width;
}

int Register::getSize() {
	return this->size;
}

int Register::getHashw() {
	return this->hashw;
}

void Register::addScratch(int bits) {
	int i;
	MAX_UNSIGNED l;

	this->width += bits;

	for ( i = 0; i < this->size; i++ ) {
		l = this->node[i]->getState() << bits;
		this->node[i]->setState(l);
	}
}

unsigned int Register::hash64(MAX_UNSIGNED key, int width) {
	unsigned int k32;

	k32 = ( key & 0xFFFFFFFF ) ^ ( key >> 32 );

	k32 *= 0x9e370001UL;
	k32 = k32 >> ( 32 - width );

	return k32;
}

Register& Register::copy() {
	Matrix m = this->toMatrix();
	Register* dst = new Register(&m, this->width);
	return *dst;
}

Matrix Register::toMatrix() {
	Matrix* m = new Matrix(1, 1 << this->width);
	int i;

	for ( i = 0; i < this->size; i++ ) {
		m->set( this->node[i]->getState(), 0, 
			this->node[i]->getAmplitude() );
	}

	return *m;
}

void Register::applyGate(Gate* g, int target) {
	g->run(this, target);
}

void Register::applyMatrix(int target, Matrix *m) {
	if ( m->getCols() == 2 && m->getRows() == 2 ) {
		this->apply2x2Matrix(target, m);
	}
}

void Register::apply2x2Matrix(int target, Matrix *m) {
	int i, j, k, iset;
	int addsize = 0, decsize = 0;
	COMPLEX_FLOAT t;
	COMPLEX_FLOAT tnot = 0;
	float limit;
	vector<bool> done;

	if ( m->getCols() != 2 || m->getRows() != 2 ) {
		Error::error(QUANTUM_EMSIZE);
	}

	if ( this->hashw ) {
		this->reconstructHash();
	}
	for ( i = 0; i < this->size; i++ ) {
		if ( this->getState( this->node[i]->getState() ^ 
			((MAX_UNSIGNED) 1 << target ) ) == -1 ) {
			addsize++;
		}
	}

	this->node.reserve(this->size + addsize);

	if ( !this->node.max_size() == this->size + addsize ) {
		Error::error(QUANTUM_ENOMEM);
	}

	//memman

	for ( i = 0; i < addsize; i++ ) {
		this->node.push_back(new Node(0,0));
	}
	
	done.resize(this->size + addsize, false);

	k = this->size;

	limit = ( 1.0 / ((MAX_UNSIGNED) 1 << this->width ) ) * epsilon;

	for ( i = 0; i < this->size; i++ ) {
		if ( !done[i] ) {
			iset = this->node[i]->getState() 
				& ( (MAX_UNSIGNED) 1 << target );
			tnot = 0;
			j = this->getState( this->node[i]->getState() 
				^ ( (MAX_UNSIGNED) 1 << target ) );
			t = this->node[i]->getAmplitude();

			if ( j >= 0 ) {
				tnot = this->node[j]->getAmplitude();
			}

			if ( iset ) {
				this->node[i]->setAmplitude(
					m->get(2, 0) * tnot + 
					m->get(3, 0) * t );
			} else {
				this->node[i]->setAmplitude( 
					m->get(0, 0) * t +
					m->get(1, 0) * tnot );
			}

			if ( j >= 0 ) {
				if ( iset ) {
					this->node[j]->setAmplitude(
						m->get(0, 0) * tnot +
						m->get(1, 0) * t );
				} else {
					this->node[j]->setAmplitude(
						m->get(2, 0) * t +
						m->get(3, 0) * tnot );
				}
			}  else {
				if ( ( m->get(1, 0) == 0 ) && (iset) ) {
					break;
				}
				if ( ( m->get(2, 0) == 0 ) && !(iset) ) {
					break;
				}
	
				this->node[k]->setState(
					this->node[i]->getState()
					^ ( (MAX_UNSIGNED) 1 << target ) );
	
				if ( iset ) {
					this->node[k]->setAmplitude(
						m->get(1, 0) * t );
				} else {
					this->node[k]->setAmplitude(
						m->get(2, 0) * t );
				}
	
				k++;
			}

			if ( j >= 0 ) {
				done[j] = 1;
			}
		}
	}

	this->size += addsize;
	
	//memman stuff

	if ( this->hashw ) {
		for ( i = 0, j = 0; i < this->size; i++ ) {
			if ( Complex::probability( 
				this->node[i]->getAmplitude() )	< limit ) {
				j++;
				decsize++;
			} else if ( j ) {
				this->node[ i - j ]->setState( 
					this->node[i]->getState() );
				this->node[ i - j ]->setAmplitude(
					this->node[i]->getAmplitude() );
			}
		}

		if ( decsize ) {
			this->size -= decsize;
			this->node.resize(this->size);

			if ( !this->node.max_size() == this->size ) {
				Error::error(QUANTUM_ENOMEM);
			}

			//memman
		}
	}
}

MAX_UNSIGNED Register::measure() {
	double r;
	int i;
	MAX_UNSIGNED result;	

	r = (double) rand() / RAND_MAX;

	for ( i = 0; i < this->size; i++ ) {
		r -= Complex::probability(this->node[i]->getAmplitude());
		if ( 0 >= r ) {
			result = this->node[i]->getState();
			this->deleteRegister();
			return result;
		}
	}
}

int Register::measure(int target) {
	int i;
	int result = 0;
	int sTarget = (MAX_UNSIGNED) 1 << target;
	double r;
	double totalZeroProb = 0;

	/* Sum the probability for zero being the result */
	for ( i = 0; i < this->size; i++ ) {
		if ( ! ( this->node[i]->getState() & sTarget ) ) {
			totalZeroProb += Complex::probability(
				this->node[i]->getAmplitude());
		}
	}

	r = (double) rand() / RAND_MAX;

	if ( r > totalZeroProb ) {
		result = 1;
	}

	this->collapse(target, result);

	return result;	
}

int Register::measure(int target, bool preserve) {
	if (!preserve) {
		return this->measure(target);
	}
}

int Register::getState(MAX_UNSIGNED a) {
	int i;
	
	if ( !this->hashw ) {
		return a;
	}

	i = Register::hash64(a, this->hashw);

	while ( this->hash[i] ) {
		if ( this->node[ this->hash[i] - 1 ]->getState() == a ) {
			return this->hash[i] - 1;
		}
		i++;
		if ( i == ( 1 << this->hashw) ) {
			i = 0;
		}
	}
	return (-1);
}

void Register::collapse(int target, int value) {
	int i;
	for ( i = 0; i < this->size; i++ ) {
		if ( (int)(( this->node[i]->getState() >> (target) ) % 2 )
			!= value ) {
			this->node[i]->setAmplitude(0);
		}
	}
	this->normalize();
}

void Register::normalize() {
	int i;
	double totalProbability = 0;

	for ( i = 0; i < this->size; i++ ) {
		totalProbability += Complex::probability(
			this->node[i]->getAmplitude());
	}

	for ( i = 0; i < this->size; i++ ) {
		this->node[i]->setAmplitude(
			this->node[i]->getAmplitude() *
			1 / (float) sqrt(totalProbability));
	}
}

void Register::addToHash(MAX_UNSIGNED a, int pos) {
	int i;
	int mark = 0;

	i = this->hash64(a, this->hashw);


	while ( this->hash[i] ) {
		i++;
		if ( i == ( 1 << this->hashw ) ) {
			if ( !mark ) {
				i = 0;
				mark = 1;
			} else {
				Error::error(QUANTUM_EHASHFULL);
			}
		}
	}

	this->hash[i] = pos+1;
}

int Register::bitMask(MAX_UNSIGNED a, int width, int *bits) {
	int i;
	int mask = 0;

	for ( i = 0; i < width; i++ ) {
		if ( a & ((MAX_UNSIGNED) 1 < bits[i] ) ) {
			mask += 1 << i;
		}
	}
	return mask;
}
		
void Register::reconstructHash() {
	int i;

	if ( !this->hashw ) {
		return;
	}

	for ( i = 0; i < ( 1 << this->hashw ); i++ ) {
		this->hash[i] = 0;
	}

	for ( i = 0; i < this->size; i++ ) {
		this->addToHash(this->node[i]->getState(), i);
	}
}

void Register::destroyHash() {
	free(this->hash);
	//memman
	this->hash = 0;
}

void Register::deleteRegister() {
	if ( this->hashw && this->hash ) {
		this->destroyHash();
	}
	this->node.erase(this->node.begin(), this->node.end());
	this->node.clear();
	this->node.resize(0);
	//memman
}

void Register::deleteRegisterOnly() {
	this->node.clear();
	this->node.resize(0);
	//memman
}

void Register::print() {
	int i, j;

	this->reconstructHash();

	for ( i = 0; i < this->size; i++ ) {
		Node *n = this->node[i];
		printf("% f %+fi|%lli> (%e) (|", 
			Complex::real(n->getAmplitude()),
			Complex::imaginary(n->getAmplitude()), n->getState(),
			Complex::probability(n->getAmplitude()));
		for ( j = this->width - 1; j >= 0; j-- ) {
			if ( j % 4 == 3 ) {
				printf(" ");
			}
			if ( (((MAX_UNSIGNED) 1 << j ) & n->getState() ) > 0 ) {
				printf("1");
			} else {
				printf("0");
			}
		}
		printf(">)\n");
	}
	printf("\n");
}

//	void printExpn();
//	void printHash();
//	void printTimeop(int width, void f(Register *));

//void might not be right here, we might want to drop a qureg
//	void stateCollapse(int bit, int value);
/*
 * Functions for manipulating multiple registers.  Probably don't need?
 */
/*
 * static Register kronecker(Register *reg1, Register *reg2);
 * static COMPLEX_FLOAT dotProduct(Register *reg1, Register *reg2);
 * static Register vectorAdd(Register *reg1, Register *reg2);
 * void vectorAddInPlace(Register *reg1, Register *reg2);
 * static Register matrixRegister(Register A(MAX_UNSIGNED, double), 
 *					double t, Register *reg);
 * void scalarRegister(COMPLEX_FLOAT r, Register *reg);
 */
}

