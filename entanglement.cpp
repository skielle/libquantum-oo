/*
 * entanglement.cpp
 */
#include <memory>
#include <stdio.h>
#include "complex.h"
#include "defs.h"
#include "entanglement.h"
#include "entangledRegister.h"
#include "entangledPair.h"

namespace Quantum {

Entanglement Entanglement::createEntanglement(MAX_UNSIGNED init, int width) {
	Entanglement e;
	e.entanglements = new EntangledPair[width];
	for ( int i = 0; i < width; i++ ) {
		e.entanglements[i] = EntangledPair();
	}
	e.aleph = new EntangledRegister(init, width, 
		make_shared<Entanglement>(e));
	e.aleph->setAleph(true);
	e.beit = new EntangledRegister(init, width,
		make_shared<Entanglement>(e));
	e.beit->setAleph(false);

	e.isAleph__stub = false;
	e.isBeit__stub = false;

	return e;
}

EntangledRegister* Entanglement::getAleph() {
	if ( this->isAleph__stub ) {
		Error::error(QUANTUM_REMOTEOP);
	}
	return this->aleph;
}

EntangledRegister* Entanglement::getBeit() {
	if ( this->isBeit__stub ) {
		Error::error(QUANTUM_REMOTEOP);
	}
	return this->beit;
}

void Entanglement::makeAlephRemote() {
	this->isAleph__stub = true;
	//this->aleph = new stub__NetworkRegister(shared_from_this());
}

void Entanglement::makeBeitRemote() {
	this->isBeit__stub = true;
	//this->beit = new stub__NetworkRegister(shared_from_this());
}

void Entanglement::entangle(int target, EntangledPair entanglementData) {
	this->entanglements[target] = entanglementData;
}

EntangledPair Entanglement::getEntanglement(int target) {
	return this->entanglements[target];
}

bool Entanglement::isEntangled(int target) {
	return !(this->entanglements[target].isNull());
}

void Entanglement::measured(bool isAleph, int target, int result){
	printf("Measured: %i\n", result);

	if ( isAleph ) {
		this->beit->pairMeasured(target, result);
	} else {
		this->aleph->pairMeasured(target, result);
	}
}
}		
