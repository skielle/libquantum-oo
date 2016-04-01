/*
 * stateVector.cpp
 */
#include <algorithm>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include "channelService_client.h"
#include "matrix.h"
#include "qubit.h"
#include "qubitMap.h"
#include "remotePeer.h"
#include "stateVector.h"
#include "stateVectorOperation.h"

using namespace std;

namespace Quantum {
StateVector::StateVector() : StateVector(1) {
}

StateVector::StateVector(int bitWidth) 
	: qsv(1, pow(2, bitWidth)), remoteQubits(bitWidth) {
	this->qsv.set(0, 0, 1);
}

StateVector::StateVector(Matrix m) 
	: qsv(m), remoteQubits(log(m.getRows())/log(2)) {
}

void StateVector::setIndex(int newIndex) {
	this->index = newIndex;
}

int StateVector::getIndex() {
	return this->index;
}

void StateVector::applyOperation(Matrix operation, int input1) {
	vector<int> inputs(1);
	inputs.at(0) = input1;
	this->applyOperation(operation, inputs);
}

void StateVector::resize(int newSize) {
	this->remoteQubits.resize(newSize);
}

void StateVector::applyOperation(Matrix operation, int input1, int input2) {
	vector<int> inputs(2);
	inputs.at(0) = input1;
	inputs.at(1) = input2;
	this->applyOperation(operation, inputs);
}

void StateVector::applyOperation(Matrix operation, vector<int> inputs) {
	Matrix expandedOperation = operation;
	unsigned int i; 
	vector<int> rowMap = generateRowMap(inputs);
	Matrix scratch( this->qsv.getCols(), this->qsv.getRows() );

	for ( i = 0; i < rowMap.size(); i++ ) {
		scratch.set(0, rowMap.at(i), this->qsv.get(0, i));
	}

	for ( i = inputs.size(); i < this->getWidth(); i++ ) {
		expandedOperation = Matrix::matrixTensor(Matrix::identity(),
			 expandedOperation);
	}

	scratch = Matrix::matrixMultiply(expandedOperation, scratch);

	for ( i = 0; i < rowMap.size(); i++ ) {
		this->qsv.set(0, i, scratch.get(0, rowMap.at(i)));
	}
	this->reduce();
	this->opHistory.push_back(StateVectorOperation(operation, inputs));
}

void StateVector::applyOperation(Matrix operation, 
	vector< shared_ptr<Qubit> > inputs) {
	unsigned int i, j;
	vector<int> inputPositions(inputs.size());

	QubitMap* m = QubitMap::getInstance();

	//first, collect all the qubit state vectors into this vector
	for ( i = 0; i < inputs.size(); i++ ) {
		int inputIndex = inputs.at(i)->v->index;
		if ( this->index != inputIndex ) {
			Matrix temp = Matrix::matrixTensor( 
					inputs.at(i)->v->qsv, this->qsv);

			for ( j = 0; j < m->numQubits(); j++ ) {
				if ( inputIndex 
					== m->getQubit(j)->v->index ) {
					m->getQubit(j)->position =+ 
						this->getWidth();
					m->getQubit(j)->v = 
						shared_from_this();
				}
			}
			this->qsv = temp;
		}
	}

	//next, convert the vector of Qubits to a vector of ints and process
	for ( i = 0; i < inputs.size(); i++ ) {
		inputPositions.at(i) = inputs.at(i)->position;
	}
	this->applyOperation(operation, inputPositions);
	this->resize(this->getWidth());
}

vector<int> StateVector::generateRowMap(vector<int> inputs) {
	vector<int> positionMap(this->getWidth(),-1);
	vector<int> rowMap(this->qsv.getRows(),0);
	int i, j, zWidth;

	zWidth = this->getWidth() - 1;

	for ( i = 0; i < inputs.size(); i++ ) {
		positionMap.at(inputs.at(i)) = i;
	}

	for ( j = 0; j < positionMap.size(); j++ ) {
		if ( positionMap.at(j) == -1 ) {
			positionMap.at(j) = i;
			i++;
		}
	}
			
	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		rowMap.at(i) = 0;
		for ( j = 0; j < positionMap.size(); j++ ) {
			rowMap.at(i) += ((( i >> (zWidth - j) ) % 2) 
					<< (zWidth - positionMap.at(j)));
		}
	}

	return rowMap;
}

int StateVector::getWidth() {
	return log(this->qsv.getRows()) / log(2);
}

void StateVector::print() {
	printf("Index: %i\r\n", this->index);
	this->qsv.print();
}

void StateVector::reduce() {
	int i, j, k, firstValueFound, currentValue;
	double DOUBLE_ZERO = .000001;
	int zWidth = this->getWidth() - 1;
	bool isBitEntangled;

	QubitMap* m = QubitMap::getInstance();

	if ( this->getWidth() == 1 ) {
		return;
	}

	for ( i = this->getWidth() - 1; i >= 0; i-- ) {
		firstValueFound = -1;
		isBitEntangled = false;
		int zI = this->getWidth() - 1 - i;
		for (j = 0; j < this->qsv.getRows(); j++ ) {
			currentValue = StateVector::isBitSet(j, zI);
			if ( abs(this->qsv.get(0, j)) > DOUBLE_ZERO ) {
				if ( firstValueFound == -1 ) {
					firstValueFound = currentValue; 
				} else {
					if ( currentValue != firstValueFound ){
						isBitEntangled = true;
					}
				}
			}
		}
		if ( !isBitEntangled && this->getWidth() > 1 
			&& firstValueFound != -1 ) {
			Matrix scratch( this->qsv.getCols(), 
				this->qsv.getRows() / 2 );

			j = 0;
			for ( k = 0; k < this->qsv.getRows(); k++ ) {
				if ( 
				(firstValueFound == 1 
					&& StateVector::isBitSet(k, zI) )
				|| (firstValueFound == 0 
					&& !StateVector::isBitSet(k, zI) ) 
				) {
					scratch.set(0, j, this->qsv.get(0, k));
					j++;
				}
			}
			this->qsv = scratch;

			for ( j = 0; j < m->numQubits(); j++ ) {
				if ( m->getQubit(j)->v->index 
					== this->index && 
					m->getQubit(j)->position == i ) {
					m->getQubit(j)->init();

					if ( firstValueFound == 1 ) {
						Matrix sigmaX = Matrix(2,2);
						sigmaX.set(0, 1, 1);
						sigmaX.set(1, 0, 1);
						m->getQubit(j)->applyMatrix(sigmaX);
					}
				}
				if ( m->getQubit(j)->v->index
					== this->index && 
					m->getQubit(j)->position > i ) {
					m->getQubit(j)->position--;
				} 
			}
		}
	}
}

bool StateVector::isBitSet(int index, int position) {
	return (int ( index / pow(2, (position)) ) 
			% 2 == 1 );
}

double StateVector::getAlpha(int position) {
	double alpha = 0.0;
	int i;

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		if ( !isBitSet(i, position) ) {
			alpha += pow(real(qsv.get(0, i)), 2) +
				pow(imag(qsv.get(0, i)), 2);
		}

	}

	return alpha;
}	

double StateVector::getBeta(int position) {
	double beta = 0.0;
	int i;

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		if ( isBitSet(i, position) ) {
			beta +=	pow(real(qsv.get(0, i)), 2) +
				pow(imag(qsv.get(0, i)), 2);
		}
	}

	return sqrt(beta);
}	

int StateVector::measure(int position) {
	int value = 0;
	double measurement = rand() / (float)RAND_MAX;

	if ( this->getAlpha(position) < measurement ) {
		value = 1;
	}

	return this->measure(position, value);
}

int StateVector::measure(int position, int forceResult) {
	this->measure(position, forceResult, true);
}

int StateVector::measure(int position, int forceResult, 
	bool propagate) {
	int i;
	vector<int> peersNotified;
	Matrix initialState = this->qsv;

	int zPosition = this->getWidth() - 1 - position;

	if ( forceResult != 0 ) {
		forceResult = 1;
	}

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		if ( forceResult == 0 && this->isBitSet(i, zPosition) ) {
			this->qsv.set(0, i, 0);
		}
		if ( forceResult == 1 && !this->isBitSet(i, zPosition) ) {
			this->qsv.set(0, i, 0);
		}
	}

	if ( propagate ) {
		for ( i = 0; i < this->getWidth(); i++ ) {
			if ( this->remoteQubits.at(i).remoteSystem 
				!= "" ) {
				QuantumChannel::ChannelService_client
				csc(this->remoteQubits.at(i)
					.remoteSystem,
					this->remoteQubits.at(i)
					.remotePort);

 				csc.SendMeasurementMessage(
					this->getIndex(), 
					position, initialState, forceResult);
			}
		}
	}
	
	this->reduce();
	this->normalize();
	return forceResult;
}

void StateVector::normalize() {
	int i;

	double total = 0.0;

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		total += pow(abs(this->qsv.get(0, i)),2);
	}

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		this->qsv.set(0, i, sqrt(pow(this->qsv.get(0, i), 2)/total));
	}
}

void sync() {

}

Matrix StateVector::toMatrix() {
	return this->qsv;
}

void StateVector::replay() {
	int i = 0;

	for ( i = 0; i < this->opHistory.size(); i++ ) {
		this->applyOperation(
			this->opHistory.at(i).getOperation(),
			this->opHistory.at(i).getArgs()
		);
	}
	this->opHistory.clear();
}

}
