#include <stdio.h>
#include <math.h>
#include "register.h"
#include "matrix.h"
#include "gates.cpp"

using namespace std;
using namespace Quantum;

int main() {
	Matrix m = Matrix(1, 4);

	m.set(0, 0, .25);
	m.set(1, 0, .25);
	m.set(2, 0, .25);
	m.set(3, 0, .25);

	m.print();

	Register r = Register(&m, 4);

	r.print();

	RotateX* rx = new RotateX();

	r.applyGate(rx, 0);

	return 0;
}
