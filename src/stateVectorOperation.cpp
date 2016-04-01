/*
 * stateVectorOperation.cpp
 */

#include <vector>

#include "matrix.h"
#include "stateVectorOperation.h"

using namespace std;

namespace Quantum {
StateVectorOperation::StateVectorOperation(Matrix operation, vector<int> args): 
	operation(operation.getCols(), operation.getRows()) 
{
	this->operation = operation;
	this->args = args;
}

QuantumMessage::StateVectorOperationMessage StateVectorOperation::serialize() {
	QuantumMessage::StateVectorOperationMessage saveMessage;
	int i;

	saveMessage.mutable_operation()->CopyFrom(this->operation.serialize());

	for ( i = 0; i < this->args.size(); i++ ) {
		saveMessage.add_arguments(this->args.at(i));
	}

	return saveMessage;
}

StateVectorOperation StateVectorOperation::unserialize(
	const QuantumMessage::StateVectorOperationMessage* loadMessage) {
	int i;
	
	QuantumMessage::MatrixMessage opMessage = loadMessage->operation();
	Matrix operation = Matrix::unserialize(&opMessage);
	vector<int> arguments(loadMessage->arguments_size());

	for ( i = 0; i < loadMessage->arguments_size(); i++ ) {
		arguments.at(i) = loadMessage->arguments(i);
	}

	StateVectorOperation historyEntry = StateVectorOperation (
		operation, arguments);

	return historyEntry;
}

Matrix StateVectorOperation::getOperation() {
	return this->operation;
}

vector<int> StateVectorOperation::getArgs() {
	return this->args;
}

}
