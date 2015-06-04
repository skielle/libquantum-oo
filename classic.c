/*
 * Classic.c
 */

#include <math.h>
#include "classic.h" 
using namespace std;

		static int ipow ( int base, int exponent );
		static int gcd ( int num1, int num2 );

int QuantumClassic::ipow( int base, int exponent ) {
	int i;
	int r = 1;

	for (i = 0; i < exponent; i++) {
		r = r * base;
	}

	return r;
}

int QuantumClassic::gcd( int num1, int num2 ) {
	int r;

	while(num2) {
		r = num1 % num2;
		num1 = num2;
		num2 = r;
	}
	return num1;
}

void QuantumClassic::fractionalApproximation ( int *n1, int *n2, int width ) {
	float f = (float) *n1 / *n2;
	float g = f;
	int i;
	int num = 0;
	int den = 0;
	int num1 = 1;
	int den1 = 0;
	int num2 = 0;
	int den2 = 1;

	do {
		i = (int) (g + 0.000005);
		g -= i-0.000005;
		g = 1.0 / g;

		if ( i * den1 + den2 > 1<<width ) {
			break;
		}

		num = i * num1 + num2;
		den = i * den1 + den2;

		num2 = num1;
		den2 = den1;
		num1 = num;
		den1 = den;
	} while ( fabs(((double) num / den) - f ) > 1.0 / (2 * (1 << width)));

	*n1 = num;
	*n2 = den;
}

int QuantumClassic::getWidth ( int n ) {
	int i;

	for ( i = 1; 1<<i < n; i++ );

	return i;
}

int QuantumClassic::inverseMod ( int n, int g ) {
	int i;

	for ( i = 1; ( i * g ) % n != 1; i++ );

	return i;
}
