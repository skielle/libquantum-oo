#include <stdio.h>
#include <math.h>
#include "register.h"
#include "matrix.h"
#include "gates.cpp"

using namespace std;
using namespace Quantum;

int main() {
	Register r = Register((MAX_UNSIGNED) 0, 4);

	r.printSystem();

	RotateX* rx = new RotateX();

	rx->setPsi( pi / 2 );

	for ( int i = 1; i <= 4; i++) {
		r.applyGate(rx, 0);
		printf("Rotation by %i pi/2:\n", i);
		r.printSystem();
	}

	SigmaX* sx = new SigmaX();
	r.applyGate(sx, 0);
	printf("Sigma X\n");
	r.printSystem();
	r.applyGate(sx, 0);
	printf("Sigma X\n");
	r.printSystem();

	SigmaY* sy = new SigmaY();
	r.applyGate(sy, 0);
	printf("Sigma Y\n");
	r.printSystem();
	r.applyGate(sy, 0);
	printf("Sigma Y\n");
	r.printSystem();

	SigmaZ* sz = new SigmaZ();
	r.applyGate(sz, 0);
	printf("Sigma Z\n");
	r.printSystem();
	r.applyGate(sz, 0);
	printf("Sigma Z\n");
	r.printSystem();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.printSystem();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.printSystem();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.printSystem();

	printf("Sigma Y ( Sigma X )\n");
	r.applyGate(sx, 0);
	r.applyGate(sy, 0);
	r.printSystem();
	return 0;
}
