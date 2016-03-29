/*
 * rotationError.cpp
 */
#include <math.h>
#include <memory>
#include <unistd.h>
#include <stdio.h>

#include "rotationError.h"
#include "matrix.h"
#include "qubit.h"

using namespace std;

namespace Quantum {
void RotationError::doEvil(shared_ptr<Qubit> q) {
	Matrix RE(2,2);

	RE.set(0, 0, cos(this->theta));
	RE.set(0, 1, -1*sin(this->theta));
	RE.set(1, 0, sin(this->theta));
	RE.set(1, 1, cos(this->theta));
	q->applyMatrix(RE);
}

void RotationError::setTheta(float newTheta) {
	this->theta = newTheta;
}

float RotationError::getTheta() {
	return this->theta;
}

}
