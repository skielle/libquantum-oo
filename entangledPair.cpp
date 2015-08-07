/*
 * entangledPair.cpp
 */
#include <stdio.h>
#include "defs.h"
#include "entangledPair.h"
#include "complex.h"
#include "matrix.h"
#include "error.h"
#include "math.h"

namespace Quantum {
EntangledPair::EntangledPair(COMPLEX_FLOAT a_00, COMPLEX_FLOAT a_10, COMPLEX_FLOAT a_01, COMPLEX_FLOAT a_11) {
	this->_isNull = true;
	this->setEntanglements(a_00, a_10, a_01, a_11);
}

EntangledPair::EntangledPair() {
	this->_isNull = true;
}
	
COMPLEX_FLOAT EntangledPair::get(int a, int b) {
	return this->entanglementAmplitudes->get(a, b);
}
void EntangledPair::setEntanglements(COMPLEX_FLOAT a_00, COMPLEX_FLOAT a_01, 
			COMPLEX_FLOAT a_10, COMPLEX_FLOAT a_11) {
	if ( !this->_isNull ) {
		Error::error(QUANTUM_EIMUTEAMP);
	}

	float totalProbability =  Complex::probability(a_00) 
		+ Complex::probability(a_01) + Complex::probability(a_10) 
		+ Complex::probability(a_11);
	if ( fabsf(totalProbability - 1.0) > float_zero ) {
		printf("Bad probability sum: %f \n", totalProbability); 
		Error::error(QUANTUM_EBADAMP);
	} else {
			this->entanglementAmplitudes = new Matrix(2,2);
			this->entanglementAmplitudes->set(0, 0, a_00);
			this->entanglementAmplitudes->set(1, 0, a_10);
			this->entanglementAmplitudes->set(0, 1, a_01);
			this->entanglementAmplitudes->set(1, 1, a_11);
			this->_isNull = false;
	}
}

bool EntangledPair::isNull() {
	return this->_isNull;
}

QuantumMessage::EntangledPairMessage EntangledPair::serialize() {
	QuantumMessage::EntangledPairMessage saveMessage;

	saveMessage.set__isnull(this->isNull());

	if ( !(this->isNull()) ) {
		saveMessage.mutable_amplitudes()->CopyFrom(
			this->entanglementAmplitudes->serialize());
	}

	return saveMessage;
}

EntangledPair& EntangledPair::unserialize(
	const QuantumMessage::EntangledPairMessage* loadMessage) {
	
	EntangledPair* ep = new EntangledPair();
	if ( !loadMessage->_isnull() ) {
		QuantumMessage::MatrixMessage mmAmps = 
			loadMessage->amplitudes();
		Matrix mAmps = Matrix::unserialize(&mmAmps);
		ep->entanglementAmplitudes = &mAmps;
		ep->_isNull = false;
	}
	
	
	return *ep;
}
}
