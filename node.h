/*
 * node.h
 */

#include "config.h"

#ifndef __QUANTUM_NODE_H
#define __QUANTUM_NODE_H

namespace Quantum {
class Node {
	COMPLEX_FLOAT amplitude;
	MAX_UNSIGNED state;

	public:
		Node ( COMPLEX_FLOAT amplitude, MAX_UNSIGNED state );
		void setAmplitude ( COMPLEX_FLOAT amplitude );
		COMPLEX_FLOAT getAmplitude ();
		void setState ( MAX_UNSIGNED state );
		MAX_UNSIGNED getState();
};
}
#endif
