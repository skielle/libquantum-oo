/*
 * entangledPair.h
 */
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
		vector<Matrix> alphaHistory;
		vector<Matrix> betaHistory;

	public:
		EntangledPair(int target, COMPLEX_FLOAT a_00, 
			COMPLEX_FLOAT a_01, COMPLEX_FLOAT a_10, 
			COMPLEX_FLOAT a_11);
		COMPLEX_FLOAT get(int a, int b);

		int getTarget();
};
}
