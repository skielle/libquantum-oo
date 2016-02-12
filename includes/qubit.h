/*
 * qubit.h
 */

#include <complex>
#include <memory>

#include "matrix.h"
#include "qubitMapEntry.h"

#ifndef __QUANTUM_BIT_H
#define __QUANTUM_BIT_H

using namespace std;

namespace Quantum {
class Qubit {
	shared_ptr<QubitMapEntry> qme;

	public:
		Qubit();
		void applyMatrix(Matrix m);
		int measure();
		void print();

		complex<double> getAlpha();
		complex<double> getBeta();
};
}
#endif
