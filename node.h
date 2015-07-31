/*
 * node.h
 * A node represents a possible state of a quantum register.  A node is /not/
 * equivilent to a qubit.  Rather, it is a possible value of the register,
 * based on the agregation of the probabilities of the qubits in the register
 * to match the value of state.  The probability of this particular state is 
 * the square of the amplitude
 */
#include "config.h"

#ifndef __QUANTUM_NODE_H
#define __QUANTUM_NODE_H

using namespace std;

namespace Quantum {
class Node {
	//squareroot of the probabilty of the register being in this state
	COMPLEX_FLOAT amplitude;
	//a possible aggregate value of the bits in this register
	MAX_UNSIGNED state;

	public:
		//create a node with a given amplitude and state
		Node ( COMPLEX_FLOAT amplitude, MAX_UNSIGNED state );
		//set the amplitude of the node
		void setAmplitude ( COMPLEX_FLOAT amplitude );
		//get the amplitude of the node
		COMPLEX_FLOAT getAmplitude ();
		//set the state of the node
		void setState ( MAX_UNSIGNED state );
		//get the state of the node
		MAX_UNSIGNED getState();
};
}
#endif
