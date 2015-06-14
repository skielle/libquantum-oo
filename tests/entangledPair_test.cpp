#include "../entangledPair.h"
#include <stdio.h>
#include <math.h>
#include <exception>
#include "../complex.h"
#include "../error.h"

using namespace std;
using namespace Quantum;

int main() {
	EntangledPair* e1 = new EntangledPair(.5, .5i, .5i, .5);
	EntangledPair* e2 = new EntangledPair(1/sqrt(2), 0, 0, 1/sqrt(2)); 	
	try {
		EntangledPair* e3 = new EntangledPair(1, 1, 0, 0);
	} catch (exception& e) {
		printf("%s\n", e.what());
	}

	printf("First pair of entangled qubits:\n");
	printf("|00> = %f + %fi\n", Complex::real(e1->get(0, 0)),
		Complex::imaginary(e1->get(0, 0)));
	printf("|10> = %f + %fi\n", Complex::real(e1->get(1, 0)),
		Complex::imaginary(e1->get(1, 0)));
	printf("|01> = %f + %fi\n", Complex::real(e1->get(0, 1)),
		Complex::imaginary(e1->get(0, 1)));
	printf("|11> = %f + %fi\n", Complex::real(e1->get(1, 1)),
		Complex::imaginary(e1->get(1, 1)));

	printf("Second pair of entangled qubits:\n");
	printf("|00> = %f + %fi\n", Complex::real(e2->get(0, 0)),
		Complex::imaginary(e2->get(0, 0)));
	printf("|10> = %f + %fi\n", Complex::real(e2->get(1, 0)),
		Complex::imaginary(e2->get(1, 0)));
	printf("|01> = %f + %fi\n", Complex::real(e2->get(0, 1)),
		Complex::imaginary(e2->get(0, 1)));
	printf("|11> = %f + %fi\n", Complex::real(e2->get(1, 1)),
		Complex::imaginary(e2->get(1, 1)));
	
	return 0;
}
