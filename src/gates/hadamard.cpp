/*
 * hadamard.cpp
 */

#include "matrix.h"
#include "hadamard.h"

namespace Quantum {
Hadamard::Hadamard() : Matrix(2,2) {
	this->set(0,0,1/sqrt(2));
	this->set(0,1,1/sqrt(2));
	this->set(1,0,1/sqrt(2));
	this->set(1,1,-1/sqrt(2));
}
}
