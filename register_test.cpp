#include <stdio.h>
#include <math.h>
#include "register.h"
#include "matrix.h"

using namespace std;
using namespace Quantum;

int main() {
	Register s = Register((MAX_UNSIGNED) 0, 4);

	Matrix m = Matrix(2, 2);

	m.set(0, 0, sqrt(1.0/2));
	m.set(1, 0, sqrt(1.0/2));
	m.set(2, 0, sqrt(1.0/2));
	m.set(3, 0, -1 * sqrt(1.0/2));

	m.print();
	s.print();
	s.applyMatrix(0, &m);
	s.applyMatrix(1, &m);
	s.applyMatrix(2, &m);
	s.applyMatrix(3, &m);
	s.print();

	printf("measured: %i\n", s.measure() );	

	s.print();

	return 0;
}
