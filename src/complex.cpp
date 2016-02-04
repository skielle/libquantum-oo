/*
 * complex.c
 */

#include <math.h>
#include "complex.h"
#include "config.h"

namespace Quantum {
COMPLEX_FLOAT Complex::conjugate ( COMPLEX_FLOAT a ) {
	float r, i;

	r = Complex::real(a);
	i = Complex::imaginary(a);

	return r - IMAGINARY * i;
}

float Complex::probability ( COMPLEX_FLOAT a ) {
	return Complex::probability_inline(a);
}

COMPLEX_FLOAT Complex::complexExponential ( float phi ) {
	return cos(phi) + IMAGINARY * sin(phi);
}

float Complex::real ( COMPLEX_FLOAT a ) {
	float *p = (float *) &a;
	return p[0];
}

float Complex::imaginary ( COMPLEX_FLOAT a ) {
	float *p = (float *) &a;
	return p[1];
}

float Complex::probability_inline ( COMPLEX_FLOAT a ) {
	float r, i;

	r = Complex::real(a);
	i = Complex::imaginary(a);

	return r * r + i * i;
}
}
