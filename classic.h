/*
 * Classic.h
 */

#ifndef __CLASSIC_H
#define __CLASSIC_H

#include <math.h>

namespace Quantum {
class Classic {
	public:
		static int ipow ( int base, int exponent );
		static int gcd ( int num1, int num2 );
		static void fractionalApproximation ( int *num1, int *num2,
				 int precision );
		static int getWidth ( int n );
		static int inverseMod ( int n, int g );
};
}
#endif
