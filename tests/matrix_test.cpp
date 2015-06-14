#include <stdio.h>
#include <math.h>
#include "matrix.h"

using namespace std;
using namespace Quantum;

int main() {
	Matrix m = Matrix(2, 2);
	Matrix n = Matrix(1, 2);

	m.set(0, 0, 1);
	m.set(0, 1, 1);
	m.set(1, 0, 0);
	m.set(1, 1, 1);

	m.print();

	n.set(0, 0, 2);
	n.set(0, 1, 2);
	n.print();

	Matrix o = Matrix::matrixMultiply(m, n);

	o.print();

	return 0;
}
