/*
 * entanglement.cpp
 */

namespace Quantum {

static Entanglement Entanglement::createEntanglement(MAX_UNSIGNED init, int width) {
	Entanglement e;
	e->entaglements = new EntangledPair[width];
	for ( int i = 0; i < width; i++ ) {
		e->entanglements[i] = NULL;
	}
	e->alpha = new EntangledRegister(init, width, e);
	e->beta = new EntangledRegister(init, width, e);

	return e;
}

EntangledRegister Entanglement::getAlpha() {
	return this->alpha;
}

EntangledRegister Entanglement::getBeta() {
	return this->beta;
}

void Entanglement::entangle(int target, EntanglementPair entanglementData) {
	this->entanglements[target] = entanglementData;
}

bool Entanglement::isEntangled(int target) {
	return this->entanglements[target] != NULL;
}

void Entanglement::measured(int target, int result);
};
}		
