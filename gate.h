/*
 * gate.h
 */

#include "config.h"
#include "node.h"

#ifndef __QUANTUM_GATE_H
#define __QUANTUM_GATE_H

namespace Quantum {
class Gate {
	public:
		virtual void run ( Node* target ) = 0;
};
}
#endif
