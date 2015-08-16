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

	shared_ptr<EntangledRegister> a = e.getAleph();
	shared_ptr<EntangledRegister> b = e.getBeit();

	EntangledPair* ab0 = new EntangledPair(1/sqrt(2), .5, 0, .5);

	Matrix m = Matrix(2, 2);

	m.set(0, 0, sqrt(1.0/2));
	m.set(1, 0, sqrt(1.0/2));
	m.set(0, 1, sqrt(1.0/2));
	m.set(1, 1, -1 * sqrt(1.0/2));

	RotateX* rx = new RotateX();
	rx->setPsi(2* pi / 4);

	a->EntangledRegister::applyMatrix(1, &m);
	b->EntangledRegister::applyMatrix(1, &m);

	e.entangle(0, *ab0);
	e.entangle(1, *ab0);
	e.entangle(2, *ab0);

	a->EntangledRegister::applyGate(rx, 1);
	a->EntangledRegister::applyGate(rx, 1);
	a->EntangledRegister::applyGate(rx, 0);
	a->EntangledRegister::applyGate(rx, 2);

	a->print();
	b->print();

	QuantumMessage::EntangledRegisterMessage erm = a->serialize();
	shared_ptr<EntangledRegister> c = EntangledRegister::unserialize(&erm);

//	a->EntangledRegister::measure(0, false);

	

	a->print();
	b->print();

	printf("C: ------ \n");
	c->print();
	printf("Is aleph? %s\n", (c->isAleph())?"true":"false");
	printf("EntangledPairs:\n");
	for ( int i = 0; i < c->getWidth(); i++ ) {
		printf("a%i : %s\n", i, (a->isEntangled(i))?"true":"false");
		printf("c%i : %s\n", i, (c->isEntangled(i))?"true":"false");
	}

	printf("OpHistory:\n");
	for ( int i = 0; i < c->getWidth(); i++ ) {
		printf("%i: \n",i);
		for ( int j = 0; j < c->getOpHistory(i)->size(); j++ ) {
			c->getOpHistory(i)->at(j).print();
		}
	}

	return 0;
}
