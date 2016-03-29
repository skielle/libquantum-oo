/*
 * ry.h 
 */

#ifndef __QUANTUM_RY
#define __QUANTUM_RY

#include "matrix.h"

namespace Quantum {
class Ry : public Matrix {
	public:
		Ry(double theta);
};
}

#endif
