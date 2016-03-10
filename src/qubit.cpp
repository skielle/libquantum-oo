/*
 * qubit.cpp
 */

#include <complex>
#include <memory>

#include "matrix.h"
#include "qubit.h"
#include "qubitMap.h"

using namespace std;

namespace Quantum {

Qubit::Qubit() {
	this->v = make_shared<StateVector>(1);
	this->position = 0;

	this->v->setIndex(QubitMap::getInstance()->getNewIndex());
}

shared_ptr<Qubit> Qubit::create() {
	shared_ptr<Qubit> m (new Qubit);

        QubitMap::getInstance()->mapEntries.push_back(m);
	return m;
}

void Qubit::applyMatrix(Matrix m) {
	this->v->applyOperation(m, this->position);
}

int Qubit::measure() {
	return this->v->measure(this->position);
}

int Qubit::measure(int forceValue) {
	return this->v->measure(this->position, forceValue);
}

void Qubit::print() {
	printf("Position: %i\r\n", this->position);
	this->v->print();
}

complex<double> Qubit::getAlpha() {
	return -1.0;
}

complex<double> Qubit::getBeta() {
	return -1.0;
}
}
