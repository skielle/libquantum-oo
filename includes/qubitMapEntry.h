/*
 * qubitMapEntry.h
 */

#include "qubitMap.h"
#include "stateVector.h"

#ifndef __QUANTUM_QUBIT_MAP_ENTRY_H
#define __QUANTUM_QUBIT_MAP_ENTRY_H

using namespace std;

namespace Quantum {
class QubitMapEntry {
	public:
		shared_ptr<StateVector> mapVector;
		int position;
};
}

#endif
