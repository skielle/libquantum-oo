/*
 * cnot.cpp
 */

#include "matrix.h"
#include "cnot.h"

namespace Quantum {
CNot::CNot() : Matrix(4,4) {
	this->set(0,0,1);
	this->set(1,1,1);
	this->set(2,3,1);
	this->set(3,2,1);
}
}
