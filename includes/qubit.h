/*
 * qubit.h
 */

#include <complex>
#include <memory>

#include "matrix.h"
#include "stateVector.h"

#ifndef __QUANTUM_BIT_H
#define __QUANTUM_BIT_H

using namespace std;

namespace Quantum {
class Qubit {
	private:
		Qubit();
		
	public:
		shared_ptr<StateVector> v;
		int position;

		static shared_ptr<Qubit> create();
		void applyMatrix(Matrix m);
		int measure();
		int measure(int forceValue);
		void print();

		complex<double> getAlpha();
		complex<double> getBeta();
};
}
#endif
