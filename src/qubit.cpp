/*
 * qubit.cpp
 */

#include <complex>
#include <memory>

#include "exceptions.h"
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

void Qubit::guardDeleted() {
	if ( this->deleted ) {
		throw E_QUBIT_UNREF;
	}
}

void Qubit::init() {
	this->deleted = false;

	this->v = make_shared<StateVector>(1);
	this->position = 0;

	this->v->setIndex(QubitMap::getInstance()->getNewIndex());
	QubitMap::getInstance()->addStateVector(this->v);
}

shared_ptr<Qubit> Qubit::create() {
	shared_ptr<Qubit> m (new Qubit);

        QubitMap::getInstance()->addQubit(m);
	return m;
}

void Qubit::applyMatrix(Matrix m) {
	this->guardDeleted();
	this->v->applyOperation(m, this->position);
}

int Qubit::measure() {
	this->guardDeleted();
	return this->v->measure(this->position);
}

int Qubit::measure(int forceValue) {
	this->guardDeleted();
	return this->v->measure(this->position, forceValue);
}

void Qubit::dereference() {
	QubitMap::getInstance()->deleteQubit(
		QubitMap::getInstance()->findQubit(
			this->v->getIndex(),
			this->position) );
	this->position = -1;
	this->v = nullptr;
	this->deleted = true;
}

void Qubit::print() {
	this->guardDeleted();
	printf("Position: %i\r\n", this->position);
	this->v->print();
}

complex<double> Qubit::getAlpha() {
	this->guardDeleted();
	return -1.0;
}

complex<double> Qubit::getBeta() {
	this->guardDeleted();
	return -1.0;
}

QuantumMessage::QubitMessage Qubit::serialize() {
	this->guardDeleted();
	QuantumMessage::QubitMessage saveMessage;

	saveMessage.set_position(this->position);
	saveMessage.set_vectorindex(this->v->getIndex());
	saveMessage.mutable_m()->CopyFrom(this->v->toMatrix().serialize());

	return saveMessage;	
}

Qubit& Qubit::unserialize(const QuantumMessage::QubitMessage* loadMessage) {
	Qubit* q = new Qubit(true);

	q->position = loadMessage->position();
	q->deleted = false;

	QuantumMessage::MatrixMessage mMessage = loadMessage->m();
	q->v = make_shared<StateVector>(Matrix::unserialize(&mMessage));
	q->v->setIndex(loadMessage->vectorindex());

	return *q;
}
}
