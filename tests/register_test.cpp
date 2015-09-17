#include <stdio.h>
#include <math.h>
#include "../register.h"
#include "../matrix.h"

using namespace std;
using namespace Quantum;

int main() {
	srand(time(NULL));

	Register s = Register((MAX_UNSIGNED) 7, 8);

	Matrix m = Matrix(2, 2);

	m.set(0, 0, sqrt(1.0/2));
	m.set(1, 0, sqrt(1.0/2));
	m.set(2, 0, sqrt(1.0/2));
	m.set(3, 0, sqrt(1.0/2));

	m.print();
printf("\r\n");
	s.print();
printf("\r\n");
	s.applyMatrix(0, &m);
	s.applyMatrix(1, &m);
	s.applyMatrix(2, &m);
	s.applyMatrix(3, &m);
	s.print();
printf("\r\n");

	printf("Measured 3: %i\n", s.measure(3));
	printf("Measured 2: %i\n", s.measure(2));
	printf("Measured 1: %i\n", s.measure(1));
	printf("Measured 0: %i\n", s.measure(0));
printf("\r\n");
	
	s.print();

	return 0;
}
