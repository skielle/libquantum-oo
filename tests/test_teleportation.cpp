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
	shared_ptr<Qubit> t1 = Qubit::create();
	shared_ptr<Qubit> t2 = Qubit::create();
	shared_ptr<Qubit> psi = Qubit::create();

	Hadamard h;
	SigmaX sx;
	Ry r(M_PI/8);

	Matrix CNOT = Matrix(4,4);
	CNOT.set(0, 0, 1);
	CNOT.set(1, 1, 1);
	CNOT.set(2, 3, 1);
	CNOT.set(3, 2, 1);

	psi->applyMatrix(r);
	printf("psi: \r\n");
	psi->print();

	vector< shared_ptr<Qubit> > inputs;
	inputs.push_back(t1);
	inputs.push_back(t2);
	t1->applyMatrix(h);
	t1->v->applyOperation(CNOT, inputs);

	inputs.at(0) = psi;
	inputs.at(1) = t1;
	t1->v->applyOperation(CNOT, inputs);

	psi->applyMatrix(h);

	int t1_i = t1->measure();
	int psi_i = psi->measure();

	printf("t1: %i, psi: %i\r\nt2:\r\n", t1_i, psi_i);
	t2->print();

	return 0;
}
