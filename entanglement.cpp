/*
 * entanglement.cpp
 */
#include <memory>
#include <stdio.h>
#include "channelService_client.h"
#include "complex.h"
#include "defs.h"
#include "entanglement.h"
#include "entangledRegister.h"
#include "entangledPair.h"
#include "quantumMessage.pb.h"

namespace Quantum {

Entanglement Entanglement::createEntanglement(MAX_UNSIGNED init, int width) {
	shared_ptr<Entanglement> e = shared_ptr<Entanglement>(new Entanglement());
	e->entanglements = new EntangledPair[width];
	for ( int i = 0; i < width; i++ ) {
		e->entanglements[i] = EntangledPair();
	}
	e->aleph = shared_ptr<EntangledRegister> (
		new EntangledRegister(init, width, e));
	e->aleph->setAleph(true);
	e->beit = shared_ptr<EntangledRegister> (
		new EntangledRegister(init, width, e));
	e->beit->setAleph(false);

	e->isAleph__stub = false;
	e->isBeit__stub = false;

	return *e;
}

shared_ptr<EntangledRegister> Entanglement::getAleph() {
	if ( this->isAleph__stub ) {
		Error::error(QUANTUM_REMOTEOP);
	}
	return this->aleph;
}

shared_ptr<EntangledRegister> Entanglement::getBeit() {
	if ( this->isBeit__stub ) {
		Error::error(QUANTUM_REMOTEOP);
	}
	return this->beit;
}

void Entanglement::setAleph(shared_ptr<EntangledRegister> _aleph) {
	this->aleph = _aleph;
}

void Entanglement::setBeit(shared_ptr<EntangledRegister> _beit) {
	this->beit = _beit;
}

void Entanglement::makeAlephRemote(int css) {
	this->isAleph__stub = true;
}

void Entanglement::makeBeitRemote(int css) {
	this->isBeit__stub = true;
}

void Entanglement::makeAlephRemote(
	shared_ptr<QuantumChannel::ChannelService_client> csc) {
	this->isAleph__stub = true;
printf("Makin' aleph remote\n");
	this->aleph__stub = csc;
	//this->aleph = new stub__NetworkRegister(shared_from_this());
}

void Entanglement::makeBeitRemote(
	shared_ptr<QuantumChannel::ChannelService_client> csc) {
printf("Makin' beit remote\n");
	this->isBeit__stub = true;
	this->beit__stub = csc;
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
		if ( this->isBeit__stub ) {
			printf("Beit is a stub.\n");
			QuantumMessage::EntangledMeasurementMessage
			pairMeasuredMessage = createMeasurementMessage(
				false, target, result, 
				aleph->getOpHistory(target));
			this->beit__stub->EventPairMeasureFinish(
				pairMeasuredMessage);
		} else {
			this->beit->pairMeasured(target, result);
		}
	} else {
		if ( this->isAleph__stub ) {
			printf("Aleph is a stub.\n");
			QuantumMessage::EntangledMeasurementMessage
			pairMeasuredMessage = createMeasurementMessage(
				true, target, result, 
				beit->getOpHistory(target));
			this->aleph__stub->EventPairMeasureFinish(
				pairMeasuredMessage);
		} else {
			this->aleph->pairMeasured(target, result);
		}
	}
}

QuantumMessage::EntangledMeasurementMessage 
	Entanglement::createMeasurementMessage(
		bool _isAleph, int target, int result, 
		vector<Matrix>* targetOpHistory) {
	QuantumMessage::EntangledMeasurementMessage createMessage;
	int i;

	createMessage.set_aleph(_isAleph);
	QuantumMessage::RegisterAddressMessage* ramTarget =
		createMessage.mutable_target();
	ramTarget->set_registeraddress(target);
	createMessage.set_result(result);

	QuantumMessage::EntangledOpHistoryMessage* history =
		createMessage.mutable_ophistory();

	for ( i = 0; i < targetOpHistory->size(); i++ ) {
		QuantumMessage::MatrixMessage* operation = 
			history->add_operations();
		operation->CopyFrom(targetOpHistory->at(i).serialize());
	}

	return createMessage;
}
}		
