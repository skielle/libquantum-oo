#include <stdio.h>
#include <math.h>
#include "../register.h"
#include "../entanglement.h"
#include "../entangledPair.h"
#include "../entangledRegister.h"
#include "../matrix.h"
#include "../gates.cpp"

using namespace std;
using namespace Quantum;

int main() {
	Entanglement e = Entanglement::createEntanglement((MAX_UNSIGNED) 0, 4);

	EntangledRegister* a = e.getAleph();
	EntangledRegister* b = e.getBeit();

	EntangledPair* ab0 = new EntangledPair(sqrt(3)/2.0, 0, 0, 0.5);

	Matrix m = Matrix(2, 2);

	m.set(0, 0, sqrt(1.0/2));
	m.set(1, 0, sqrt(1.0/2));
	m.set(0, 1, sqrt(1.0/2));
	m.set(1, 1, -1 * sqrt(1.0/2));

	m.print();

	Matrix sigX = Matrix(2, 2);
	sigX.set(0, 0, 0);
	sigX.set(1, 0, 1);
	sigX.set(0, 1, 1);
	sigX.set(1, 1, 0);

	sigX.print();

	RotateX* rx = new RotateX();
	rx->setPsi(pi / 3);

	a->EntangledRegister::applyMatrix(0, &m);
	b->EntangledRegister::applyMatrix(0, &m);

	e.entangle(0, *ab0);

	a->print();

	a->EntangledRegister::applyMatrix(0, &sigX);
	a->EntangledRegister::applyGate(rx, 0);

	a->print();
	b->print();

	a->EntangledRegister::measure(0, false);
	a->print();
	b->print();

	return 0;
}
