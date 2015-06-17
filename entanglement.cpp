/*
 * entanglement.cpp
 */
#include <stdio.h>
#include "complex.h"
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
	e.aleph = new EntangledRegister(init, width, &e);
	e.aleph->setAleph(true);
	e.beit = new EntangledRegister(init, width, &e);
	e.beit->setAleph(false);

	return e;
}

EntangledRegister* Entanglement::getAleph() {
	return this->aleph;
}

EntangledRegister* Entanglement::getBeit() {
	return this->beit;
}

void Entanglement::entangle(int target, EntangledPair entanglementData) {
	this->entanglements[target] = entanglementData;
}

bool Entanglement::isEntangled(int target) {
	return this->entanglements[target].isNull();
return true;
}

void Entanglement::measured(bool isAleph, int target, int result){
	printf("Measured: %i\n", result);
	float p0, p1;

	if ( isAleph ) {
		p0 = Complex::probability(
			this->entanglements[target].get(result, 0));
		p1 = Complex::probability(
			this->entanglements[target].get(result, 1));
		this->beit->updateAmplitudes(target, p0, p1);
	} else {
		printf("Measured from beit.  Aleph (0): %f, (1): %f\n",
			Complex::probability(
				this->entanglements[target].get(0, result) ),
			Complex::probability(
				this->entanglements[target].get(1, result) ) );
	}
	//need to know aleph or beit to do lookup
}
}		
