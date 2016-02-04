#include <fstream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
//#include "../quantumMessage.pb.h"
//#include "../complex.h"
#include "matrix.h"

using namespace std;
using namespace Quantum;

int main() {
//	fstream output("/tmp/pb.mx.s", ios::out | ios::trunc | ios::binary);
	Matrix m = Matrix(2, 2);
	Matrix n = Matrix(1, 2);
	Matrix ident = Matrix(2, 2);

	m.set(0, 0, sqrt(.5));
	m.set(0, 1, sqrt(.5));
	m.set(1, 0, sqrt(.5));
	m.set(1, 1, -1 * sqrt(.5));

	ident.set(0, 0, 1);
	ident.set(0, 1, 0);
	ident.set(1, 0, 0);
	ident.set(1, 1, 1);

/*
printf("Testing serialization\n");
printf("before:\n");
m.print();
	QuantumMessage::MatrixMessage m_m = m.serialize();
	Matrix m_um = Matrix::unserialize(&m_m);
printf("after:\n");
m_um.print();

printf("serialization done.\n");
*/
	Matrix p = Matrix::inverse(m);

//	p.serialize(output);

//	output.close();

	printf("m\n");
	m.print();

	printf("p=m^-1\n");
	p.print();
	
	n.set(0, 0, 2);
	n.set(0, 1, 2);
	printf("n\n");
	n.print();

	Matrix o = Matrix::matrixMultiply(m, n);
	printf("m*n\n");
	o.print();

	Matrix z = Matrix::matrixTensor(m, ident);
	printf("I X m\n");
	z.print();
	
	return 0;
}
