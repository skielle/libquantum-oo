/*
 * iEvil.h
 */
#ifndef __QUANTUM_IEVIL_H
#define __QUANTUM_IEVIL_H

#include "iRegister.h"
#include "systemMessage.h"

namespace Quantum {
class iEvil {
	public:
		virtual void doEvil(shared_ptr<iRegister> reg, SystemMessage message) = 0; 
};
}
#endif
