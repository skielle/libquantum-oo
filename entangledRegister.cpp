/*
 * entangledRegister.cpp
 */
#include <math.h>
#include "entanglement.h"
#include "entangledRegister.h"
#include "register.h"
#include "types.h"

namespace Quantum {
EntangledRegister::EntangledRegister(MAX_UNSIGNED init, int width, Entanglement* ent):
Register(init, width)
{
	this->ent = ent;
	this->opHistory = new vector<Matrix>* [width];
}

void EntangledRegister::apply2x2Matrix(int target, Matrix *m) {
	Register::apply2x2Matrix(target, m);
	if ( ent->isEntangled(target) ) {
		this->opHistory[target]->push_back(*m);
	}
}

int EntangledRegister::measure(int target, bool preserve) {
	int result = Register::measure(target, preserve);

	this->ent->measured(this->aleph, target, result);

	return result;
}

vector<Matrix>* EntangledRegister::getOpHistory(int target) {
	return this->opHistory[target];
}

bool EntangledRegister::getAleph() {
	return this->aleph;
}

void EntangledRegister::setAleph(bool aleph) {
	this->aleph = aleph;
} 

void EntangledRegister::updateAmplitudes(int target, float p0, float p1) {
	int i;
	for ( i = 0; i < this->size; i++ ) {
		if ( (int)(( this->node[i]->getState() >> (target) ) % 2 ) == 0 ) {
			this->node[i]->setAmplitude( 
				this->node[i]->getAmplitude() * sqrt(p0) );
		} else {
			this->node[i]->setAmplitude(
				this->node[i]->getAmplitude() * sqrt(p1) );
		}
	}
	this->normalize();
}
}
