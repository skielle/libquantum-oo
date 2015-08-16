/*
 * stub__networkRegister.h
 */

#ifndef __QUANTUM_STUB__NETWORKREGISTER_H
#define __QUANTUM_STUB__NETWORKREGISTER_H

#include <memory>

#include "entangledRegister.h"

namespace Quantum {
class stub__NetworkRegister {
	protected:
		shared_ptr<Entanglement> ent;
		bool _isAleph;

	public:
		stub__NetworkRegister(shared_ptr<Entanglement> ent);
		void setAleph(bool aleph);
		bool isAleph();

		void pairMeasured(int target, int result);
};
}
#endif
