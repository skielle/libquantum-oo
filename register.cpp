/*
 * register.cpp
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <vector>

#include "classic.h"
#include "classicRegister.h"
#include "complex.h"
#include "config.h"
#include "defs.h"
#include "error.h"
#include "matrix.h"
#include "qubit.h"
#include "register.h"
#include "quantumMessage.pb.h"

using namespace std;

namespace Quantum {
Register::Register() {
	Error::error(QUANTUM_EOPCODE);
}

Register::Register(Matrix *m, int width) {
	this->fromMatrix(m);
}

Register::Register(Matrix *m) {
	this->fromMatrix(m);
}

void Register::fromMatrix(Matrix *m) {
	int i;
	Matrix* temp = new Matrix(2, 2);
	temp->set(0, 1, 0);
	temp->set(1, 1, 0);

	if ( m->getCols() != 1 ) {
		Error::error(QUANTUM_EMCMATRIX);
	}

	qubits.resize(m->getRows()/2);

	for ( i = 0; i < qubits.size(); i++ ) {
		qubits.at(i) = new QuBit();
		if ( m->get(0, 2*i) != 0 || m->get(0, 2*i + 1) != 0 ) {
			temp->set(0, 0, m->get(0, 2*i));
			temp->set(1, 0, m->get(0, 2*i + 1));
			qubits.at(i)->applyMatrix(temp);
		}
	}
}

Register::Register(MAX_UNSIGNED initval, int width) {
	ClassicRegister* cr = new ClassicRegister(width);
	cr->setValue(initval);
	this->fromClassicRegister(cr);
}

Register::Register(ClassicRegister *cr) { 
	this->fromClassicRegister(cr);
}

void Register::fromClassicRegister(ClassicRegister *cr) {
	int i;
	Matrix* m_one = new Matrix(2, 2);
	m_one->set(0, 0, 0);
	m_one->set(0, 1, 0);
	m_one->set(1, 0, 1);
	m_one->set(1, 1, 0);

	qubits.resize(cr->getWidth());

	for ( i = 0; i < qubits.size(); i++ ) {
		qubits.at(i) = new QuBit();
		if ( cr->getBit(i) != 0 ) {
			qubits.at(i)->applyMatrix(m_one);
		}
	}
}	

int Register::getWidth() {
	return this->qubits.size();
}

Register& Register::copy() {
	Matrix m = this->toMatrix();
	Register* dst = new Register(&m, this->qubits.size());
	return *dst;
}

Matrix Register::toMatrix() {
	Matrix* m = new Matrix(1, this->qubits.size() * 2);
	int i;

	for ( i = 0; i < this->qubits.size(); i++ ) {
		m->set(0, 2*i, this->qubits.at(i)->getAlpha() );
		m->set(0, 2*i + 1, this->qubits.at(i)->getBeta() );
	}

	return *m;
}

void Register::applyGate(Gate* g, int target) {
	g->run(this, target);
}

void Register::applyMatrix(int target, Matrix *m) {
	if ( m->getCols() == 2 && m->getRows() == 2 ) {
		this->apply2x2Matrix(target, m);
	}
}

void Register::apply2x2Matrix(int target, Matrix *m) {
	this->qubits.at(target)->applyMatrix(m);
}

int Register::measure() {
	int i;
	int total;

	for ( i = 0; i < this->qubits.size(); i++ ) {
		total += this->qubits.at(i)->measure() * Classic::ipow(2, i);
	}

	return total;
}

int Register::measure(int target) {
	return this->qubits.at(target)->measure();	
}

int Register::measure(int target, bool preserve) {
	return this->measure(target);
}

void Register::print() {
	int i;
	for ( i = 0; i < this->qubits.size(); i++ ) {
		this->qubits.at(i)->print();
	}
}

QuantumMessage::RegisterMessage Register::serialize() {
	QuantumMessage::RegisterMessage saveMessage;

	saveMessage.mutable_m()->CopyFrom(this->toMatrix().serialize());
	saveMessage.set_width(this->qubits.size());

	return saveMessage;
}

Register& Register::unserialize(
	const QuantumMessage::RegisterMessage* loadMessage) {
	QuantumMessage::MatrixMessage mMessage = loadMessage->m();

	Matrix m = Matrix::unserialize(&mMessage);

	Register* ret = new Register(&m, loadMessage->width());
	return *ret;
}	
}
