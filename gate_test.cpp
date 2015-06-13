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
	rx->setPsi(pi / 2);
	r.applyGate(rx, 0);
	printf("Rotation by pi/2:\n");
	r.print();

	SigmaX* sx = new SigmaX();
	r.applyGate(sx, 0);
	printf("Sigma X\n");
	r.print();
	r.applyGate(sx, 0);
	printf("Sigma X\n");
	r.print();

	SigmaY* sy = new SigmaY();
	r.applyGate(sy, 0);
	printf("Sigma Y\n");
	r.print();
	r.applyGate(sy, 0);
	printf("Sigma Y\n");
	r.print();

	SigmaZ* sz = new SigmaZ();
	r.applyGate(sz, 0);
	printf("Sigma Z\n");
	r.print();
	r.applyGate(sz, 0);
	printf("Sigma Z\n");
	r.print();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.print();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.print();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.print();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.print();

	return 0;
}
