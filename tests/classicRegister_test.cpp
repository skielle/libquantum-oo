#include <stdio.h>
#include "../classicRegister.h"

using namespace std;
using namespace Quantum;

int main() {
	ClassicRegister a = ClassicRegister(7, 8);

	a.setBit(1, 0);

	a.print();

	return 0;
}
