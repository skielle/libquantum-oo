/*
 * stateVectorOperation.h
 */

#include <vector>

#include "matrix.h"

#ifndef __QUANTUM_STATE_VECTOR_OPERATION
#define __QUANTUM_STATE_VECTOR_OPERATION

namespace Quantum {
class StateVectorOperation {
        Matrix operation;
        vector<int> args;

	public:
		StateVectorOperation(Matrix operation, vector<int> args);
		QuantumMessage::StateVectorOperationMessage serialize();
		static StateVectorOperation unserialize(
			const QuantumMessage::StateVectorOperationMessage* 
			loadMessage);
		Matrix getOperation();
		vector<int> getArgs();
};		
}
#endif
