/*
 * qubit.h
 */
#include "config.h"
#include "matrix.h"

#ifndef __QUANTUM_BIT_H
#define __QUANTUM_BIT_H

using namespace std;

namespace Quantum {
class QuBit {
	//alpha and beta
	COMPLEX_FLOAT alpha, beta;

	public:
		QuBit ();
		void reset();
		void applyMatrix(Matrix* m);
		void normalize();
		int measure();
		void print();
		COMPLEX_FLOAT getAlpha();
		COMPLEX_FLOAT getBeta();
};
}
#endif
