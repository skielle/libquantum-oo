#include <fstream>
#include <stdio.h>
#include <math.h>
#include "../complex.h"
#include "../matrix.h"

using namespace std;
using namespace Quantum;

int main() {
//	fstream output("/tmp/pb.mx.s", ios::out | ios::trunc | ios::binary);
	Matrix m = Matrix(2, 2);
	Matrix n = Matrix(1, 2);

	m.set(0, 0, sqrt(.5));
	m.set(0, 1, sqrt(.5));
	m.set(1, 0, sqrt(.5));
	m.set(1, 1, -1 * sqrt(.5));

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

//	fstream input("/tmp/pb.mx.s", ios::in | ios::binary);
//	Matrix q = Matrix::unserialize(input);
//	printf("unserialize(serialize(p))\n");
//	q.print();

	Matrix z = Matrix(3, 3);

	z.set(0, 0, 1);
	z.set(1, 0, 1);
	z.set(2, 0, 2);
	
	z.set(0, 1, 2);
	z.set(1, 1, 3);
	z.set(2, 1, 4);

	z.set(0, 2, 3);
	z.set(1, 2, 4);
	z.set(2, 2, 5);

	z.print();

	printf("determinant(z): %f + %fi\n", 
		Complex::real(Matrix::determinant(z)),
		Complex::imaginary(Matrix::determinant(z)));

	return 0;
}
