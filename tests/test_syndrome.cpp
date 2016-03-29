#include <fstream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "qubit.h"
#include "gates.h"
#include "qubitMap.h"
//#include "../quantumMessage.pb.h"
//#include "../complex.h"
#include "stateVector.h"

using namespace std;
using namespace Quantum;

int main() {
	for ( int i = 0; i < 10; i++ ) {
	for ( int j = 0; j < 100; j++ ) {
	shared_ptr<Qubit> L1 = Qubit::create();
	shared_ptr<Qubit> L2 = Qubit::create();
	shared_ptr<Qubit> L3 = Qubit::create();
	shared_ptr<Qubit> A1 = Qubit::create();
	shared_ptr<Qubit> A2 = Qubit::create();

	Ry r_omega(M_PI / 180.0 * (double)(i * 5 + i));

	Hadamard h; 

	Matrix CNOT = Matrix(4,4);
	CNOT.set(0, 0, 1);
	CNOT.set(1, 1, 1);
	CNOT.set(2, 3, 1);
	CNOT.set(3, 2, 1);

	L1->applyMatrix(h);

	vector< shared_ptr<Qubit> > inputs(2);

	inputs.at(0) = L1;
	inputs.at(1) = L2;
	L1->v->applyOperation(CNOT, inputs);
	inputs.at(0) = L1;
	inputs.at(1) = L3;
	L1->v->applyOperation(CNOT, inputs);

	vector< shared_ptr<Qubit> > singleInput(1);
	singleInput.at(0) = L1;
	L1->v->applyOperation(r_omega, singleInput);
	singleInput.at(0) = L2;
	L2->v->applyOperation(r_omega, singleInput);
	singleInput.at(0) = L3;
	L3->v->applyOperation(r_omega, singleInput);
/*	inputs.at(1) = A1;
	inputs.at(0) = L1;
	L1->v->applyOperation(CNOT, inputs);
	inputs.at(1) = A1;
	inputs.at(0) = L2;
	L1->v->applyOperation(CNOT, inputs);
	int a1_value = A1->measure();
	inputs.at(0) = L2;
	inputs.at(1) = A2;
	L1->v->applyOperation(CNOT, inputs);
	inputs.at(0) = L3;
	inputs.at(1) = A2;
	L1->v->applyOperation(CNOT, inputs);
	int a2_value = A2->measure();
*/
	int l1_value = L1->measure();
	int l2_value = L2->measure();
	int l3_value = L3->measure();

	printf("%i, %i, %i, %i\r\n",
	i*5,	l1_value, l2_value, l3_value);
}
}
	return 0;
}
