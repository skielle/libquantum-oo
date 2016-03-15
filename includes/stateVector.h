/*
 * stateVector.h
 */

#include "matrix.h"

#ifndef __QUANTUM_STATE_VECTOR
#define __QUANTUM_STATE_VECTOR

namespace Quantum {
class Qubit;
class StateVector: public enable_shared_from_this<StateVector> {
	private:
		int index; 
		Matrix qsv;
	public:
		StateVector();
		StateVector(int bitWidth);
		StateVector(Matrix m);
		void setIndex(int newIndex);
		int getIndex();
		void applyOperation(Matrix operation, int position);
		void applyOperation(Matrix operation, int input1, int input2);
		void applyOperation(Matrix operation, vector<int> inputs);
		void applyOperation(Matrix operation, 
			vector< shared_ptr<Qubit> > inputs);
		void getQubit(int position);
		double getAlpha(int position);
		double getBeta(int position);
		int measure(int position);
		int measure(int position, int forceResult);
		void updateSystemsTable();
		void print();
		int getWidth();
		void reduce();
		bool isEntangled(int position);
		static bool isBitSet(int index, int position);
		void swapBits(int position1, int position2);
		vector<int> generateRowMap(vector<int> inputs);
		Matrix toMatrix();
};
}

#endif
