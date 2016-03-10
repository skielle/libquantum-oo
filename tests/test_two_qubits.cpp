#include <fstream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
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

	Matrix CNOT = Matrix(4,4);
	CNOT.set(0, 0, 1);
	CNOT.set(1, 1, 1);
	CNOT.set(2, 3, 1);
	CNOT.set(3, 2, 1);

	vector< shared_ptr<Qubit> > inputs;
	inputs.push_back(d);
	inputs.push_back(c);

	c->v->applyOperation(CNOT, inputs);

	c->print();
	d->print();

	return 0;
}
