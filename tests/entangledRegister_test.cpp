#include <stdio.h>
#include <math.h>
#include "../register.h"
#include "../entanglement.h"
#include "../entangledPair.h"
#include "../entangledRegister.h"
#include "../matrix.h"

using namespace std;
using namespace Quantum;

int main() {
	Entanglement e = Entanglement::createEntanglement((MAX_UNSIGNED) 0, 4);

	EntangledRegister* a = e.getAleph();
	EntangledRegister* b = e.getBeit();

	EntangledPair* ab0 = new EntangledPair(sqrt(3)/2.0, 1.0/2, 0, 0);

	e.entangle(0, *ab0);

	Matrix m = Matrix(2, 2);

	m.set(0, 0, sqrt(1.0/2));
	m.set(1, 0, sqrt(1.0/2));
	m.set(2, 0, sqrt(1.0/2));
	m.set(3, 0, -1 * sqrt(1.0/2));

	m.print();
	a->applyMatrix(0, &m);
	b->applyMatrix(0, &m);

	a->print();
	b->print();
	a->measure(0, true);
	a->print();
	b->print();

	return 0;
}
