/*
 * complex.h
 */

#ifndef __QUANTUM_COMPLEX_H
#define __QUANTUM_COMPLEX_H

#include "config.h"

namespace Quantum {
class Complex {
	public:
		static COMPLEX_FLOAT conjugate( COMPLEX_FLOAT a );
		static float probability ( COMPLEX_FLOAT a );
		static COMPLEX_FLOAT complexExponential ( float phi );
		static float real ( COMPLEX_FLOAT a );
		static float imaginary ( COMPLEX_FLOAT a );
		static float probability_inline ( COMPLEX_FLOAT a );
};
}
#endif
