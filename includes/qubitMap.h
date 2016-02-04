/*
 * qubitMap.h
 */

#ifndef __QUANTUM_QUBIT_MAP_H
#define __QUANTUM_QUBIT_MAP_H

#include <memory>
#include <vector>

#include "qubitMapEntry.h"
#include "stateVector.h"

using namespace std;

namespace Quantum {
class QubitMap {
	private:
		vector< shared_ptr<QubitMapEntry> > mapEntries;
		QubitMap();
		static QubitMap* mapInstance;
	public:
		static QubitMap* getInstance();
		static shared_ptr<QubitMapEntry> createQubit();
};
}
#endif
