/*
 * qubit.cpp
 */

#include <complex>
#include <memory>

#include "matrix.h"
#include "qubit.h"
#include "qubitMap.h"
#include "qubitMapEntry.h"

using namespace std;

namespace Quantum {
Qubit::Qubit() {
	this->qme = QubitMap::createQubit();
}

void Qubit::applyMatrix(Matrix m) {
	this->qme->mapVector->applyOperation(m, this->qme->position);
}

int Qubit::measure() {
	return -1;
}

void Qubit::print() {
	this->qme->mapVector->print();
}

complex<double> Qubit::getAlpha() {
	return -1.0;
}

complex<double> Qubit::getBeta() {
	return -1.0;
}
}
