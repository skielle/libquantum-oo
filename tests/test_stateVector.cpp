#include <fstream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <vector>

#include "stateVector.h"

using namespace std;
using namespace Quantum;

int main() {
	StateVector qs = StateVector(3);

	Matrix sigmax = Matrix(2,2);
	sigmax.set(0, 1, 1);
	sigmax.set(1, 0, 1);

	Matrix hadamard = Matrix(2,2);
	hadamard.set(0,0,1/sqrt(2));
	hadamard.set(0,1,1/sqrt(2));
	hadamard.set(1,0,-1/sqrt(2));
	hadamard.set(1,1,1/sqrt(2));

	Matrix cnot = Matrix(4,4);
	cnot.set(0, 0, 1);
	cnot.set(1, 1, 1);
	cnot.set(2, 3, 1);
	cnot.set(3, 2, 1);	

	qs.print();

//	qs.applyOperation(sigmax, 0);
	qs.applyOperation(sigmax, 1);
	qs.applyOperation(sigmax, 2);
	qs.print();

//	qs.applyOperation(cnot, 1, 0);
//	qs.print();

	vector<int> inputs(1,0);
	inputs.at(0) = 2;
	qs.applyOperation(sigmax, inputs);
	qs.print();
	return 0;
}
