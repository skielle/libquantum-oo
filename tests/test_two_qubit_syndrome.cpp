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
	double theta = M_PI / 3.0;
	double omega = M_PI / 180.0;

	printf("Initial rotation by %f, error rotation of %f\r\n", theta, omega);

	shared_ptr<Qubit> L1 = Qubit::create();
	shared_ptr<Qubit> L2 = Qubit::create();
	shared_ptr<Qubit> A1 = Qubit::create();

	Matrix kakRotate = Matrix(2,2);
	kakRotate.set(0, 0, cos(theta));
	kakRotate.set(0, 1, sin(theta));
	kakRotate.set(1, 0, -sin(theta));
	kakRotate.set(1, 1, cos(theta));

	Matrix errorRotate = Matrix(2,2);
	errorRotate.set(0, 0, cos(omega));
	errorRotate.set(0, 1, sin(omega));
	errorRotate.set(1, 0, -sin(omega));
	errorRotate.set(1, 1, cos(omega));

	Matrix CNOT = Matrix(4,4);
	CNOT.set(0, 0, 1);
	CNOT.set(1, 1, 1);
	CNOT.set(2, 3, 1);
	CNOT.set(3, 2, 1);

	printf("Initial state of L1:\r\n");
	L1->print();
	L1->applyMatrix(kakRotate);
	
	printf("Rotated L1:\r\n");
	L1->print();

	vector< shared_ptr<Qubit> > inputs(2);

	inputs.at(0) = L1;
	inputs.at(1) = L2;
	L1->v->applyOperation(CNOT, inputs);

	printf("CNOT L1, L2:\r\n");
	L1->print();

	vector< shared_ptr<Qubit> > singleInput(1);
	singleInput.at(0) = L1;

	L1->v->applyOperation(errorRotate, singleInput);
	printf("Error rotation on L1:\r\n");
	L1->print();

	singleInput.at(0) = L2;

	L2->v->applyOperation(errorRotate, singleInput);
	printf("Error rotation on L2:\r\n");
	L2->print();

	printf("Ancillary Qubits:\r\n");
	printf("CNOT L1 A1:\r\n");
	inputs.at(1) = A1;
	inputs.at(0) = L1;
	L1->v->applyOperation(CNOT, inputs);
	A1->print();
	printf("CNOT L2 A1:\r\n");
	inputs.at(1) = A1;
	inputs.at(0) = L2;
	L1->v->applyOperation(CNOT, inputs);
	A1->print();
	printf("Measure A1:\r\n");
	A1->measure(0);
	A1->print();

	return 0;
}
