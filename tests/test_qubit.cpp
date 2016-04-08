#include <fstream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "amplitudeDamping.h"
#include "gates.h"
#include "qubit.h"
#include "qubitMap.h"
//#include "../quantumMessage.pb.h"
//#include "../complex.h"
#include "stateVector.h"

using namespace std;
using namespace Quantum;

int main() {
	shared_ptr<Qubit> c = Qubit::create();
	shared_ptr<Qubit> d = Qubit::create();

	Matrix sigmax = Matrix(2,2);
	sigmax.set(0, 1, 1);
	sigmax.set(1, 0, 1);

	c->print();
	c->applyMatrix(sigmax);
	c->print();

	d->print();

	Hadamard h;
	d->applyMatrix(h);
	d->print();

	Matrix rho = d->v->toDensity();
	rho.print();

	d->v->fromDensity(rho);

	d->print();

	AmplitudeDamping er;
	er.setEta(.10);

	er.doEvil(d);

	d->print();	

	return 0;
}
