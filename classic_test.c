#include <stdio.h>
#include "classic.h"
using namespace std;

int main() {
	int a = 2;
	int b = 10;
	printf("%i ^ %i = %i\n", a, b, QuantumClassic::ipow(a, b));
	
	a = 150;
	b = 500;
	printf("gcd(%i, %i) = %i\n", a, b, QuantumClassic::gcd(a, b));

	int c = a;
	int d = b;
	QuantumClassic::fractionalApproximation(&c, &d, 20);
	printf("fractional approximation %i/%i = %i/%i\n", a, b, c, d );

	printf("Width %i = %i\n", a, QuantumClassic::getWidth(a));

	a = 7;
	b = 9;
	printf("mod^-1(%i) %i = %i\n", a, b, QuantumClassic::inverseMod(a, b));

	return 0;
}
