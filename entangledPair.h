/*
 * entangledPair.h
 */
#ifndef __QUANTUM_ENTANGLED_PAIR_H
#define __QUANTUM_ENTANGLED_PAIR_H

#include "matrix.h"

namespace Quantum {
class EntangledPair {
	protected:
		/* entanglementAmplitudes:
		 *      0     1
		 * --T-----T-----
		 * 0 |a_00 |a_01 
		 * 1 |a_10 |a_11
		 *
		 * a_00|00> + a_01|01> + a_10|10> + a_11|11>
		 */
		Matrix* entanglementAmplitudes;
		int target;
		bool _isNull;

	public:
		EntangledPair(COMPLEX_FLOAT a_00, COMPLEX_FLOAT a_01, 
			COMPLEX_FLOAT a_10, COMPLEX_FLOAT a_11);
		EntangledPair();
		COMPLEX_FLOAT get(int a, int b);
		void setEntanglements(COMPLEX_FLOAT a_00, COMPLEX_FLOAT a_01, 
			COMPLEX_FLOAT a_10, COMPLEX_FLOAT a_11);
		int getTarget();
		bool isNull();
};
}
#endif
