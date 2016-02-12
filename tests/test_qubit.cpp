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
	Qubit c = Qubit();

	Matrix sigmax = Matrix(2,2);
	sigmax.set(0, 1, 1);
	sigmax.set(1, 0, 1);

	c.print();
	c.applyMatrix(sigmax);
	c.print();

	return 0;
}
