/*
 * entangledPair.cpp
 */
#include <stdio.h>
#include "entangledPair.h"
#include "complex.h"
#include "matrix.h"
#include "error.h"
#include "math.h"

namespace Quantum {
EntangledPair::EntangledPair(COMPLEX_FLOAT a_00, COMPLEX_FLOAT a_10, COMPLEX_FLOAT a_01, COMPLEX_FLOAT a_11) {
	float totalProbability =  Complex::probability(a_00) 
		+ Complex::probability(a_01) + Complex::probability(a_10) 
		+ Complex::probability(a_11);
	if ( fabsf(totalProbability - 1.0) > .0000001 ) {
		printf("Bad probability sum: %f \n", totalProbability); 
		Error::error(QUANTUM_EBADAMP);
	} else {
			this->entanglementAmplitudes = new Matrix(2,2);
			this->entanglementAmplitudes->set(0, 0, a_00);
			this->entanglementAmplitudes->set(1, 0, a_10);
			this->entanglementAmplitudes->set(0, 1, a_01);
			this->entanglementAmplitudes->set(1, 1, a_11);
	}
}
	
COMPLEX_FLOAT EntangledPair::get(int a, int b) {
	return this->entanglementAmplitudes->get(a, b);
}
}
