/*
 * qureg.h
 */

#ifndef __QUANTUM_REGISTER_H

#define __QUANTUM_REGISTER_H

#include <sys/types.h>
#include <vector>
#include "config.h"
#include "defs.h"
#include "matrix.h"
#include "error.h"
#include "node.h"
using namespace std;

namespace Quantum {
class Gate;

class Register {
	int width;
	int size;
	int hashw;
	vector<Node*> node;
	int *hash;

	public:
		Register(Matrix *m, int width);
		Register(MAX_UNSIGNED initval, int width);
		Register(int n, int width);

		void addScratch(int bits);
		unsigned int hash64(MAX_UNSIGNED key, int width);

		Register& copy();
		Matrix toMatrix();
		void applyGate(Gate* g, int target);
		void applyMatrix(int target, Matrix *m);
		void apply2x2Matrix(int target, Matrix *m);
		MAX_UNSIGNED measure();
		int measure(int target);
		int measure(int target, bool preserve);

		int getState(MAX_UNSIGNED a);
		void addToHash(MAX_UNSIGNED a, int pos);
		int bitMask(MAX_UNSIGNED a, int width, int *bits);
		
		void reconstructHash();
		void destroyHash();
		void deleteRegister();
		void deleteRegisterOnly();

		void print();
		void printExpn();
		void printHash();
		void printTimeop(int width, void f(Register *));

		//void might not be right here, we might want to drop a qureg
		void stateCollapse(int bit, int value);

		/*
		 * Functions for manipulating multiple registers.  
		 * Probably don't need?
		 */
		/*
		 * static Register kronecker(Register *reg1, Register *reg2);
		 * static COMPLEX_FLOAT 
		 *	dotProduct(Register *reg1, Register *reg2);
		 * static Register vectorAdd(Register *reg1, Register *reg2);
		 * void vectorAddInPlace(Register *reg1, Register *reg2);
		 * static Register 
		 *	matrixRegister(Register A(MAX_UNSIGNED, double), 
		 *				double t, Register *reg);
		 * void scalarRegister(COMPLEX_FLOAT r, Register *reg);
	 	 */
};

class Gate {
	public:
                virtual void run ( Register* r, int target ) = 0;
};

}

#endif
