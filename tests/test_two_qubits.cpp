#include <fstream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "qubit.h"
#include "qubitMap.h"
#include "stateVector.h"
#include "gates.h"

using namespace std;
using namespace Quantum;

int main() {
	shared_ptr<Qubit> c = Qubit::create();
	shared_ptr<Qubit> d = Qubit::create();
	shared_ptr<Qubit> e = Qubit::create();

	Ry r(M_PI/3);

	c->applyMatrix(r);

	Matrix CNOT = Matrix(4,4);
	CNOT.set(0, 0, 1);
	CNOT.set(1, 1, 1);
	CNOT.set(2, 3, 1);
	CNOT.set(3, 2, 1);

	vector< shared_ptr<Qubit> > inputs;
	inputs.push_back(c);
	inputs.push_back(d);
	c->v->applyOperation(CNOT, inputs);
	inputs.at(0) = c;
	inputs.at(1) = e;
	c->v->applyOperation(CNOT, inputs);
	c->print();
	c->v->applyOperation(CNOT, inputs);

	c->print();
	c->measure();
	c->print();

	return 0;
}
