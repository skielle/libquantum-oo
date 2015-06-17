/*
 * entanglement.h
 */
#ifndef __QUANTUM_ENTANGLEMENT_H
#define __QUANTUM_ENTANGLEMENT_H

#include "register.h"
#include "entangledRegister.h"
#include "entangledPair.h"

namespace Quantum {
class Entanglement {
	protected:
		EntangledRegister* aleph;
		EntangledRegister* beit;
		EntangledPair* entanglements;
	public:
		static Entanglement createEntanglement(MAX_UNSIGNED init, int width);
		EntangledRegister* getAleph();
		EntangledRegister* getBeit();
		void entangle(int target, EntangledPair entanglementData);

		bool isEntangled( int target);

		void applyMatrix(int target, Matrix* m);

		void measured(bool isAleph, int target, int result);
};
}
#endif		
