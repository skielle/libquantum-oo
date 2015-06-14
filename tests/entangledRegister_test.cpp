#include <stdio.h>
#include <math.h>
#include "register.h"
#include "entangledRegister.h"
#include "matrix.h"

using namespace std;
using namespace Quantum;

int main() {
	EntangledRegister s = EntangledRegister();

	Matrix m = Matrix(2, 2);

	m.set(0, 0, sqrt(1.0/2));
	m.set(1, 0, sqrt(1.0/2));
	m.set(2, 0, sqrt(1.0/2));
	m.set(3, 0, -1 * sqrt(1.0/2));

	m.print();
	s.print();

	return 0;
}
