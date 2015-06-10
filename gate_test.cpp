#include <stdio.h>
#include <math.h>
#include "register.h"
#include "matrix.h"
#include "gates.cpp"

using namespace std;
using namespace Quantum;

int main() {
	Register r = Register((MAX_UNSIGNED) 0, 4);

	r.print();

	RotateX* rx = new RotateX();
	rx->setPsi(pi / 3);
	r.applyGate(rx, 0);
	r.print();

	SigmaX* sx = new SigmaX();
	r.applyGate(sx, 0);
	r.print();
	r.applyGate(sx, 0);
	r.print();

	SigmaY* sy = new SigmaY();
	r.applyGate(sy, 0);
	r.print();
	r.applyGate(sy, 0);
	r.print();

	SigmaZ* sz = new SigmaZ();
	r.applyGate(sz, 0);
	r.print();
	r.applyGate(sz, 0);
	r.print();

	return 0;
}
