/*
 * stateVector.cpp
 */
#include <algorithm>
#include <math.h>
#include <stdlib.h>

#include "matrix.h"
#include "qubit.h"
#include "qubitMap.h"
#include "stateVector.h"

using namespace std;

namespace Quantum {
StateVector::StateVector() : StateVector(1) {
}

StateVector::StateVector(int bitWidth) 
	: qsv(1, pow(2, bitWidth)) {
	this->qsv.set(0, 0, 1);
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
		expandedOperation = Matrix::matrixTensor(Matrix::identity(), expandedOperation);
	}

	scratch = Matrix::matrixMultiply(expandedOperation, scratch);

	for ( i = 0; i < rowMap.size(); i++ ) {
		this->qsv.set(0, i, scratch.get(0, rowMap.at(i)));
	}
	this->reduce();
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

			for ( j = 0; j < m->mapEntries.size(); j++ ) {
				if ( inputIndex 
					== m->mapEntries.at(j)->v->index ) {
					m->mapEntries.at(j)->position =+ 
						this->getWidth();
					m->mapEntries.at(j)->v = 
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
			rowMap.at(i) += ((( i >> (zWidth - j) ) % 2) << (zWidth - positionMap.at(j)));
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
	vector<int> reduceBit(1);

	QubitMap* m = QubitMap::getInstance();

	if ( this->getWidth() == 1 ) {
		return;
	}

	for ( i = 0; i < this->getWidth(); i++ ) {
		firstValueFound = -1;
		isBitEntangled = false;
		for (j = 0; j < this->qsv.getRows(); j++ ) {
			currentValue = StateVector::isBitSet(j, zWidth -i);
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
		if ( !isBitEntangled ) {
			reduceBit.at(0) = i;
			vector<int> rowMap = generateRowMap(reduceBit);
			Matrix scratch( this->qsv.getCols(), 
				this->qsv.getRows() );

			for ( k = 0; k < rowMap.size(); k++ ) {
				scratch.set(0, rowMap.at(k), 
					this->qsv.get(0, k));
			}

			this->qsv = scratch;
			this->qsv.setRows(this->qsv.getRows() / 2);

			for ( j = 0; j < m->mapEntries.size(); j++ ) {
				if ( m->mapEntries.at(j)->v->index 
					== this->index && 
					m->mapEntries.at(j)->position == i ) {
					m->mapEntries.erase(
						m->mapEntries.begin()+j);
					Qubit::create();
				}
				if ( m->mapEntries.at(j)->v->index
					== this->index && 
					m->mapEntries.at(j)->position > i ) {
					m->mapEntries.at(j)->position--;
				} 
			}
		}
	}
}

bool StateVector::isBitSet(int index, int position) {
	return (int ( index / pow(2, (position)) ) % 2 == 1 );
}

double StateVector::getAlpha(int position) {
	double alpha = 0.0;
	int i;

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		if ( !isBitSet(i, position) ) {
			alpha += abs(this->qsv.get(0, i));
		}
	}

	return sqrt(alpha);
}	

double StateVector::getBeta(int position) {
	double beta = 0.0;
	int i;

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		if ( isBitSet(i, position) ) {
			beta += abs(this->qsv.get(0, i));
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
	int i;
	int zWidth = this->getWidth() - 1;
	
	if ( forceResult != 0 ) {
		forceResult = 1;
	}

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		if ( ( i >> ( zWidth - position ) ) % 2 != forceResult ) {
			this->qsv.set(0, i, 0);
		}
	}

	this->reduce();
	return forceResult;
}

}
