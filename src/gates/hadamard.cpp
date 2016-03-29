/*
 * ry.cpp
 */

#include "matrix.h"
#include "ry.h"

namespace Quantum {
Ry::Ry(double theta) : Matrix(2,2) {
	this->set(0,0,cos(theta/2));
	this->set(0,1,sin(theta/2));
	this->set(1,0,-sin(theta/2));
	this->set(1,1,cos(theta/2));
}
}
