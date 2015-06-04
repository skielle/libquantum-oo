/*
 * Classic.h
 */

#include <math.h>
using namespace std;

class QuantumClassic {
	public:
		static int ipow ( int base, int exponent );
		static int gcd ( int num1, int num2 );
		static void fractionalApproximation ( int *num1, int *num2,
				 int precision );
		static int getWidth ( int n );
		static int inverseMod ( int n, int g );
};
