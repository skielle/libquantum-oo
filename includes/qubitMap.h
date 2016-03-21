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
		int mapIndex;
		vector< shared_ptr<Qubit> > qubitEntries;
		vector< shared_ptr<StateVector> > stateVectorEntries; 
	public:
		void addQubit(Qubit q);
		void addQubit(shared_ptr<Qubit> q);
		int numQubits();
		shared_ptr<Qubit> getQubit(int i);
		void deleteQubit(int i);

		void addStateVector(StateVector v);
		void addStateVector(shared_ptr<StateVector> v);
		int numStateVectors();
		shared_ptr<StateVector> getStateVector(int i);
		void deleteStateVector(int i);

		int getNewIndex();
		static QubitMap* getInstance();
	//	static shared_ptr<Qubit> createQubit();
};
}
#endif
