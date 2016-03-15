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
	this->init();
}

Qubit::Qubit(bool isRemote) {
	if ( !isRemote ) {
		this->init();
	}
}

void Qubit::init() {
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

QuantumMessage::QubitMessage Qubit::serialize() {
	QuantumMessage::QubitMessage saveMessage;

	saveMessage.set_position(this->position);
	saveMessage.set_vectorindex(this->v->getIndex());
	saveMessage.mutable_m()->CopyFrom(this->v->toMatrix().serialize());

	return saveMessage;	
}

Qubit& Qubit::unserialize(const QuantumMessage::QubitMessage* loadMessage) {
	Qubit* q = new Qubit(true);

	q->position = loadMessage->position();

	QuantumMessage::MatrixMessage mMessage = loadMessage->m();
	q->v = make_shared<StateVector>(Matrix::unserialize(&mMessage));
	q->v->setIndex(loadMessage->vectorindex());

	return *q;
}
}
