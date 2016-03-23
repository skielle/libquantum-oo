/*
 * sigmax.cpp
 */

#include "matrix.h"
#include "sigmax.h"

namespace Quantum {
SigmaX::SigmaX() : Matrix(2,2) {
	this->set(0, 0, 0);
	this->set(1, 0, 1);
	this->set(0, 1, 1);
	this->set(1, 1, 0);
}
}
