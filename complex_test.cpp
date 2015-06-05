#include <stdio.h>
#include <math.h>
#include "complex.h"
using namespace std;
using namespace Quantum;

int main() {
	COMPLEX_FLOAT a = Complex::complexExponential( M_PI / 4 );

	printf("a = %f %fi\n", Complex::real(a), 
		Complex::imaginary(a));

	a = Complex::conjugate(a);

	printf("a = %f %fi\n", Complex::real(a), 
		Complex::imaginary(a));

	printf("probability(a) = %f\n", Complex::probability(a));
	
	return 0;
}
