/*
 * stateVector.h
 */

#include "matrix.h"

#ifndef __QUANTUM_STATE_VECTOR
#define __QUANTUM_STATE_VECTOR

namespace Quantum {

class Qubit {};

class StateVector {
	private:
		Matrix qsv;
	public:
		StateVector();
		StateVector(int bitWidth);
//		StateVector(int bitWidth, Qubit q);
		void applyOperation(Matrix operation, int position);
		void applyOperation(Matrix operation, int input1, int input2);
		void applyOperation(Matrix operation, vector<int> inputs);
		void getQubit(int position);
		int measure(int position);
		void updateSystemsTable();
		void print();
		int getWidth();
		void reduce();
		bool isEntangled(int position);
		static bool isBitSet(int index, int position);
		void swapBits(int position1, int position2);
		vector<int> generateRowMap(vector<int> inputs);
};
}

#endif
