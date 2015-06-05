/*
 * complex.c
 */

#include <math.h>
#include "complex.h"
#include "config.h"
using namespace std;

COMPLEX_FLOAT QuantumComplex::conjugate ( COMPLEX_FLOAT a ) {
	float r, i;

	r = QuantumComplex::real(a);
	i = QuantumComplex::imaginary(a);

	return r - IMAGINARY * i;
}

float QuantumComplex::probability ( COMPLEX_FLOAT a ) {
	return QuantumComplex::probability_inline(a);
}

COMPLEX_FLOAT QuantumComplex::complexExponential ( float phi ) {
	return cos(phi) + IMAGINARY * sin(phi);
}

float QuantumComplex::real ( COMPLEX_FLOAT a ) {
	float *p = (float *) &a;
	return p[0];
}

float QuantumComplex::imaginary ( COMPLEX_FLOAT a ) {
	float *p = (float *) &a;
	return p[1];
}

float QuantumComplex::probability_inline ( COMPLEX_FLOAT a ) {
	float r, i;

	r = QuantumComplex::real(a);
	i = QuantumComplex::imaginary(a);

	return r * r + i * i;
}
