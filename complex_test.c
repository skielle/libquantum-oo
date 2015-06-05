#include <stdio.h>
#include <math.h>
#include "complex.h"
using namespace std;

int main() {
	COMPLEX_FLOAT a = QuantumComplex::complexExponential( M_PI / 4 );

	printf("a = %f %fi\n", QuantumComplex::real(a), 
		QuantumComplex::imaginary(a));

	a = QuantumComplex::conjugate(a);

	printf("a = %f %fi\n", QuantumComplex::real(a), 
		QuantumComplex::imaginary(a));

	printf("probability(a) = %f\n", QuantumComplex::probability(a));
	
	return 0;
}
