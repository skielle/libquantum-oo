#include <stdio.h>
#include <fstream>
#include <math.h>
#include "qubit.h"
#include "matrix.h"

using namespace std;
using namespace Quantum;

int main() {
	QuBit* q = new QuBit();
	Matrix* m = new Matrix(2, 2);

	m->set(0, 0, 1/sqrt(2));
	m->set(0, 1, 1/sqrt(2));
	m->set(1, 0, 1/sqrt(2));
	m->set(1, 1, -1/sqrt(2));

	q->print();
	q->applyMatrix(m); 
	q->print();
	q->measure();
	q->print();

	q->reset();
	m->set(0, 0, 7);
	m->set(0, 1, 0);
	m->set(1, 0, 2);
	m->set(1, 1, 0);

	q->applyMatrix(m); 
	q->print();

	return 0;
}
