/*
 * iEvil.h
 */

#ifndef __QUANTUM_IEVIL_H
#define __QUANTUM_IEVIL_H

#include <memory>

#include "qubit.h"

namespace Quantum {
class iEvil {
	public:
		virtual void doEvil(shared_ptr<Qubit> q) = 0;
};
}
#endif
