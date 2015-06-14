/*
 * entangledRegister.h
 */

#include "register.h"
#include "types.h"
#include "matrix.h"

#ifndef __QUANTUM_ENTANGLED_REGISTER_H
#define __QUANTUM_ENTANGLED_REGISTER_H

namespace Quantum {
class EntangledRegister: public Register {
	protected:
		Entanglement ent;
	public:
		//do some stuff to modification methods
		// modifications must either:
		//	a - update the entangled state or...
		//	b - break the entanglement
		EntangledRegister(MAX_UNSIGNED init, int width, Entanglement ent);
};
}

#endif
