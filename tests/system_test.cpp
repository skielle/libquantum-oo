#include <stdio.h>
#include <math.h>
#include "../gates.cpp"
#include "../matrix.h"
#include "../register.h"
#include "../system.h"

using namespace std;
using namespace Quantum;

int main() {
	System* mySys = System::getInstance();

	Register s = Register((MAX_UNSIGNED) 0, 8);
	int key = mySys->addRegister(&s);

	Matrix m = Matrix(2, 2);

	m.set(0, 0, sqrt(1.0/2));
	m.set(1, 0, sqrt(1.0/2));
	m.set(2, 0, sqrt(1.0/2));
	m.set(3, 0, sqrt(1.0/2));

	m.print();
	s.applyMatrix(1, &m);
	s.applyMatrix(2, &m);
	s.applyMatrix(3, &m);
	s.applyGate(new SigmaX(), 0);
	s.print();

	Register* q = (Register*)(mySys->getRegister(key));
	q->print();
	
	return 0;
}
