/*
 * matrix.cpp
 */

#include <complex>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "error.h"
#include "matrix.h"
//#include "quantumMessage.pb.h"
//#include "system.h"

using namespace std;

namespace Quantum {
Matrix::Matrix(int cols, int rows) {
	this->setRows(rows);
	this->setCols(cols);

	this->t = new vector< complex<double> > ( 
		this->getRows() * this->getCols(), 0) ;

	if ( !this->t ) {
		Error::error(QUANTUM_ENOMEM);
	}
}

void Matrix::setRows(int rows) { this->rows = rows; }
void Matrix::setCols(int cols) { this->cols = cols; }
int Matrix::getRows() { return this->rows; }
int Matrix::getCols() { return this->cols; }

complex<double> Matrix::get( int x, int y ) {
	return this->t->at(x + y * this->getCols());
}

void Matrix::set( int x, int y, complex<double> v ) {
	this->t->at(x + y * this->getCols()) = v;
}

Matrix Matrix::matrixMultiply(Matrix a, Matrix b) {
	int i, j, k;
	complex<double> cVal;

	if ( a.getCols() != b.getRows() ) {
		Error::error(QUANTUM_EMSIZE);
	}

	Matrix c = Matrix( b.getCols(), a.getRows() );

	for ( i = 0; i < b.getCols(); i++ ) {
		for ( j = 0; j < a.getRows(); j++ ) {
			cVal = 0;
			for ( k = 0; k < b.getRows(); k++ ) {
				cVal += a.get( k, j ) * b.get( i, k );
			}
			c.set( i, j, cVal );
		}
	}
	return c;
}

Matrix Matrix::matrixTensor(Matrix a, Matrix b) {
	int i, j, m, n;
	complex<double> cVal;

	Matrix c = Matrix ( b.getCols() * a.getCols(), 
				b.getRows() * a.getRows() );
	
	for ( i = 0; i < b.getRows(); i++ ) {
		for ( j = 0; j < b.getCols(); j++ ) {
			for ( m = 0; m < a.getRows(); m++ ) {
				for ( n = 0; n < a.getCols(); n++ ) {
					c.set( i * a.getRows() + m,
						j * a.getCols() + n,
						b.get(j, i) * a.get(n, m) );
				}
			}
		}
	}
	return c;
}

Matrix Matrix::identity() {
	Matrix i = Matrix( 2, 2 );
	i.set(0, 0, 1);
	i.set(0, 1, 0);
	i.set(1, 0, 0);
	i.set(1, 1, 1);

	return i;
}

complex<double> Matrix::determinant(Matrix m) {
	int i, j, k, x, y;
	if ( m.getCols() != m.getRows() ) {
		Error::error(QUANTUM_EINVMATRIX);
	}

	if ( m.getCols() == 2 ) {
		return m.get(0, 0) * m.get(1, 1) 
			- m.get(0, 1) * m.get(1, 0);
	} else {
		complex<double> det = 0;
		for ( i = 0; i < m.getCols(); i++ ) {
			x = 0;
			y = 0;
			Matrix n = Matrix(m.getCols()-1, m.getCols()-1);

			for (j = 1; j < m.getCols(); j++ ) {
				for (k = 0; k < m.getCols(); k++ ) {
					if ( k != i ) {
						n.set(x, y, m.get(k, j));
						y++;
						if ( y == m.getCols() - 1 ) {
							x++;
							y = 0;
						}
					}
				}
			}
			det += m.get(i, 0) * pow(-1, i) * Matrix::determinant(n);
		}
		return det;
	}		
}

Matrix Matrix::inverse(Matrix m) {
	complex<double> detr = Matrix::determinant(m);
	if ( detr.real() * detr.real() + detr.imag() * detr.imag()  > .001 ) {
//		Error::error(QUANTUM_EINVMATRIX);
	}
	Matrix inv = Matrix(m.getRows(), m.getCols());
	if ( m.getCols() == 2 ) {
		inv.set(0, 0, m.get(1, 1) / detr );
		inv.set(1, 1, m.get(0, 0) / detr );
		inv.set(0, 1, m.get(0, 1) * -1.0 / detr );
		inv.set(1, 0, m.get(1, 0) * -1.0 / detr );
		return inv;
	}

	if ( m.getCols() == 3 ) {
	}

	if ( m.getCols() == 4 ) {
	}

	Error::error(QUANTUM_EINVMATRIX);
	return m;
}

Matrix::~Matrix() {
	this->t = 0;
}

void Matrix::print() {
	int i, j;
	int z = 0;

	while ( ( 1 << z++ ) < this->getRows() );
	z--;

	for ( i = 0; i < this->getRows(); i++ ) {
		for ( j = 0; j < this->getCols(); j++ ) {
			printf("%g %+gi, ", this->get( j, i ).real(),
				this->get( j, i ).imag() );
		}
		printf("\n");
	}
	printf("\n");
}

/**************** Serialization functions
QuantumMessage::MatrixMessage Matrix::serialize() {
	int i;
	string serializedMessage;
	QuantumMessage::MatrixMessage saveMessage;
	
	saveMessage.set_rows(this->getRows());
	saveMessage.set_cols(this->getCols());

	for ( i = 0; i < this->getRows() * this->getCols(); i++ ) {
		QuantumMessage::ComplexMessage* saveT = saveMessage.add_t();

		saveT->set_real(Complex::real(this->t->at(i)));
		saveT->set_imaginary(Complex::imaginary(this->t->at(i)));
	}

	return saveMessage;
}

Matrix Matrix::unserialize(const QuantumMessage::MatrixMessage* loadMessage) {
	int i;

	Matrix m ( loadMessage->cols(), loadMessage->rows() );

	for ( i = 0; i < loadMessage->t_size(); i++ ) {
		QuantumMessage::ComplexMessage cMessage = loadMessage->t(i);

		m.set(i % m.getCols(), i / m.getCols(), 
			 cMessage.real() + cMessage.imaginary() * IMAGINARY);
	}

	return m;
}
*/
}
