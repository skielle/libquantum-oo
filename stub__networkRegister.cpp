/*
 * stub__networkRegister.cpp
 */

#include "entangledRegister.h"
#include "entanglement.h"
#include "error.h"
#include "stub__networkRegister.h"

namespace Quantum {
stub__NetworkRegister::stub__NetworkRegister(shared_ptr<Entanglement> _ent) {
	this->ent = _ent;
}

void stub__NetworkRegister::setAleph(bool aleph) {
}

bool stub__NetworkRegister::isAleph() {
}

void stub__NetworkRegister::pairMeasured(int target, int result) {
}

}
