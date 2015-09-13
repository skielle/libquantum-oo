/*
 * qubit.cpp
 */
#include "complex.h"
#include "config.h"
#include "error.h"
#include "matrix.h"
#include "qubit.h"

using namespace std;

namespace Quantum {

QuBit::QuBit() {
	this->alpha = 1;
	this->beta = 0;
}

void QuBit::applyMatrix(Matrix* m) {
	COMPLEX_FLOAT temp;
	
	if ( m->getRows() != 2 || m->getCols() != 2 ) {
		Error::error(QUANTUM_EMSIZE);
	}

	temp = m->get(0, 0) * this->alpha + m->get(1, 0) * this->beta;
	this->beta = m->get(1, 0) * this->alpha + m->get(1, 1) * this->beta;
	this->alpha = temp;

	this->normalize();
}

void QuBit::normalize() {
}

void QuBit::measure() {
	float measurement;
	srand(time(NULL));
	measurement = rand() / (float)RAND_MAX;

	if ( measurement < Complex::probability(this->alpha) ) {
		this->alpha = 1;
		this->beta = 0;
	} else {
		this->alpha = 0;
		this->beta = 1;
	}
}

void QuBit::print() {
	printf("(%f + %fi)|0> + (%f + %fi)|1>\r\n",
		Complex::real(this->alpha),
		Complex::imaginary(this->alpha),
		Complex::real(this->beta),
		Complex::imaginary(this->beta) );
}

COMPLEX_FLOAT QuBit::getAlpha() {
	return this->alpha;
}

COMPLEX_FLOAT QuBit::getBeta() {
	return this->beta;
}

}
