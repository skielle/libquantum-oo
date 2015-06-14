/*
 * entangledRegister.cpp
 */

#include "entangledRegister.h"
#include "register.h"
#include "types.h"

namespace Quantum {
EntangledRegister::EntangledRegister(MAX_UNSIGNED init, int width, Entanglement ent):
Register(init, width)
{
	this->ent = ent;
	this->opHistory = new vector<Matrix>* [width];
}

EntangledRegister::apply2x2Matrix(int target, Matrix *m) {
	parent::apply2x2Matrix(target, m);
	if ( ent->isEntangled(target) ) {
		this->opHistory[target]->pushBack(m);
	}
}

int EntangledRegister::meassure(int target, bool preserve) {
	int result = parent::meassure(target, preserve);

	this->ent->measured(target, result);

	return result;
}

vector<Matrix> EntangledRegister::getOpHistory(int target) {
	return this->opHistory[target];
}

}
