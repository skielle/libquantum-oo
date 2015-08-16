/*
 * entangledRegister.cpp
 */
#include <stdio.h>
#include <math.h>
#include "complex.h"
#include "entanglement.h"
#include "entangledPair.h"
#include "entangledRegister.h"
#include "matrix.h"
#include "register.h"
#include "config.h"

namespace Quantum {
EntangledRegister::EntangledRegister(MAX_UNSIGNED init, int width, shared_ptr<Entanglement> newEnt):
Register(init, width)
{
	int i;
	this->ent = newEnt;
	this->opHistory = new vector<Matrix>* [width];
	for ( i = 0; i < width; i++ ) {
		this->opHistory[i] = new vector<Matrix>;
	}
}

EntangledRegister::EntangledRegister(Matrix* m, int width, shared_ptr<Entanglement> newEnt):
Register(m, width)
{
	int i;
	this->ent = newEnt;
	this->opHistory = new vector<Matrix>* [width];
	for ( i = 0; i < width; i++ ) {
		this->opHistory[i] = new vector<Matrix>;
	}
}

void EntangledRegister::apply2x2Matrix(int target, Matrix *m) {
	Register::apply2x2Matrix(target, m);
	if ( ent->isEntangled(target) ) {
		this->opHistory[target]->push_back(*m);
	}
}

int EntangledRegister::measure(int target, bool preserve) {
	int result = Register::measure(target, preserve);

	this->ent->measured(this->isAleph(), target, result);

	return result;
}

vector<Matrix>* EntangledRegister::getOpHistory(int target) {
	return this->opHistory[target];
}

bool EntangledRegister::isAleph() {
	return this->_isAleph;
}

void EntangledRegister::setAleph(bool aleph) {
	this->_isAleph = aleph;
} 

bool EntangledRegister::isEntangled(int target) {
	return this->ent->isEntangled(target);
}

shared_ptr<Entanglement> EntangledRegister::getEntanglement() {
	return this->ent;
} 

void EntangledRegister::pairMeasured(int target, int result) {
	this->revert(target);
	this->updateAmplitudes(target, result);
	this->playAltHistory(target);
	this->replay(target);
}

void EntangledRegister::updateAmplitudes(int target, int result) {
	int i;
	float p0, p1;

	if ( !this->isAleph() ) {
		p0 = Complex::probability(
			this->ent->getEntanglement(target).get(result, 0) );
		p1 = Complex::probability(
			this->ent->getEntanglement(target).get(result, 1) );
	} else {
		p0 = Complex::probability(
			this->ent->getEntanglement(target).get(0, result) );
		p1 = Complex::probability(
			this->ent->getEntanglement(target).get(1, result) );
	}

	if ( p0 + p1 > float_zero ) { 
		for ( i = 0; i < this->size; i++ ) {
			if ( (int)(( this->node[i]->getState() >> (target) ) 
				% 2 ) == 0 ) {
				this->node[i]->setAmplitude( 
					this->node[i]->getAmplitude() *
					 sqrt(p0) );
			} else {
				this->node[i]->setAmplitude(
					this->node[i]->getAmplitude() *
					 sqrt(p1) );
			}
		}
		this->normalize();
	}
}

void EntangledRegister::revert(int target) {
	int i;
	if ( !this->opHistory[target]->empty() ) {
		for ( i = this->opHistory[target]->size() - 1; i >= 0; i-- ) {
			Matrix im = Matrix::inverse(opHistory[target]->at(i));
			Register::apply2x2Matrix(target, &im);
		}
	}
}

void EntangledRegister::replay(int target) {
	int i;
	for ( i = 0; i < this->opHistory[target]->size(); i++ ) {
		Register::apply2x2Matrix(target, &this->opHistory[target]->at(i));
	}
}

void EntangledRegister::playAltHistory(int target) {
	int i;
	vector<Matrix>* altHistory;
	if ( this->isAleph() ) {
		altHistory = this->ent->getBeit()->getOpHistory(target);
	} else {
		altHistory = this->ent->getAleph()->getOpHistory(target);
	}

	for ( i = 0; i < altHistory->size(); i++ ) {
		Register::apply2x2Matrix(target, &altHistory->at(i));
	}
}

QuantumMessage::EntangledRegisterMessage EntangledRegister::serialize() {
	int i, j;
	QuantumMessage::EntangledRegisterMessage saveMessage;
	saveMessage.mutable_m()->CopyFrom(this->toMatrix().serialize());
	saveMessage.set_width(this->width);

	saveMessage.set_aleph(this->isAleph());

	for ( i = 0; i < this->width; i++ ) {
		//add entangled pairs
		QuantumMessage::EntangledPairMessage* saveEp = 
			saveMessage.add_pairs();
		QuantumMessage::EntangledPairMessage myEp = 
			this->ent->getEntanglement(i).serialize();
		saveEp->CopyFrom(myEp);
	}

	for ( i = 0; i < this->width; i++ ) {
		QuantumMessage::EntangledOpHistoryMessage* history =
			saveMessage.add_ophistory();
		for (j = 0; j < this->opHistory[i]->size(); j++ ) {
			QuantumMessage::MatrixMessage* operation =
			history->add_operations();
			operation->CopyFrom(
				this->opHistory[i]->at(j).serialize());
		}
	}

	return saveMessage;
}

EntangledRegister& EntangledRegister::unserialize(
	const QuantumMessage::EntangledRegisterMessage* loadMessage) {

	int i, j;
	QuantumMessage::MatrixMessage mMessage = loadMessage->m();
	EntangledRegister* ret;

	Matrix m = Matrix::unserialize(&mMessage);
	Entanglement e = Entanglement::createEntanglement(
		0, loadMessage->width());

	for ( i = 0; i < loadMessage->width(); i++ ) {
		if ( !( loadMessage->pairs(i)._isnull() ) ) {
			e.entangle(i, EntangledPair::unserialize(
				&(loadMessage->pairs(i))));
		}
	}

	

	ret = new EntangledRegister(&m, loadMessage->width(), 
		make_shared<Entanglement>(e));
	ret->setAleph(loadMessage->aleph());

	for ( i = 0; i < loadMessage->width(); i++ ) {
		for ( j = 0; j < loadMessage->ophistory(i).operations_size();
			j++ ) {
			ret->opHistory[i]->push_back(
				Matrix::unserialize(
				&(loadMessage->ophistory(i).operations(j))));
		}
	}

	if ( loadMessage->aleph() ) {
		e.aleph = ret;
	} else {
		e.beit = ret;
	}

	return *ret;
}

}
