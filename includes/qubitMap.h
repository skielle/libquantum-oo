/*
 * qubitMap.h
 */

#ifndef __QUANTUM_QUBIT_MAP_H
#define __QUANTUM_QUBIT_MAP_H

#include <memory>
#include <vector>

using namespace std;

namespace Quantum {
class Qubit;

class QubitMap {
	private:
		QubitMap();
		static QubitMap* mapInstance;
		int vectorIndex;
	public:
		vector< shared_ptr<Qubit> > mapEntries;

		int getNewIndex();
		static QubitMap* getInstance();
	//	static shared_ptr<Qubit> createQubit();
};
}
#endif
