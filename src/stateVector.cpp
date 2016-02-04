/*
 * stateVector.cpp
 */
#include <math.h>

#include "matrix.h"
#include "stateVector.h"

using namespace std;

namespace Quantum {
StateVector::StateVector() : StateVector(1) {
}

StateVector::StateVector(int bitWidth) 
	: qsv(1, pow(2, bitWidth)) {
	this->qsv.set(0, 0, 1);
} 

void StateVector::applyOperation(Matrix operation, int input1) {
	Matrix expandedOperation(0, 0);
	int i;

	if ( input1 == 0 ) {
		expandedOperation = operation;
	} else {
		expandedOperation = Matrix::identity();
	}

	for ( i = 1; i < this->getWidth(); i++ ) {
		if ( i == input1 ) {
			expandedOperation = 
				Matrix::matrixTensor(expandedOperation, 
					operation);
		} else {
			expandedOperation =
				Matrix::matrixTensor(expandedOperation, 
					Matrix::identity());
		}
	}

	this->qsv = Matrix::matrixMultiply(expandedOperation, this->qsv);
}

void StateVector::applyOperation(Matrix operation, int input1, int input2) {
	Matrix expandedOperation = operation;
	int i;

	this->swapBits(0, input1);
	if ( input1 > input2 ) {
		this->swapBits(1, input2+1);
	} else {
		this->swapBits(1, input2);
	}

	for ( i = 2; i < this->getWidth(); i++ ) {
		expandedOperation = Matrix::matrixTensor(expandedOperation,
			Matrix::identity());
	}
	this->qsv = Matrix::matrixMultiply(expandedOperation, this->qsv);
	this->swapBits(0, input1);
	this->swapBits(0, input1);
	this->swapBits(1, input2);
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
		expandedOperation = Matrix::matrixTensor(expandedOperation,
			Matrix::identity());
	}

	scratch = Matrix::matrixMultiply(expandedOperation, scratch);

	for ( i = 0; i < rowMap.size(); i++ ) {
		this->qsv.set(0, i, scratch.get(0, rowMap.at(i)));
	}
}

vector<int> StateVector::generateRowMap(vector<int> inputs) {
	vector<int> rowMap(this->qsv.getRows(),0);
	int i, j, k;

	for ( j = 0; j < inputs.size(); j++ ) {
		for ( k = j; k < inputs.size(); k++ ) {
			if ( inputs.at(j) > inputs.at(k) ) {
				inputs.at(k)++;
			}
		}
	}

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		rowMap.at(i) = i; 
		for ( j = 0; j < inputs.size(); j++ ) {
			rowMap.at(i) = 
( ( rowMap.at(i) >> ( inputs.at(j) + 1 ) ) << ( inputs.at(j) + 1 ) ) |
( ( rowMap.at(i) & ( ( 1 << inputs.at(j) ) - 1 ) ) << 1 ) |
( ( rowMap.at(i) >> ( inputs.at(j) ) ) & 1 );
		
		}
	}

	return rowMap;
}

int StateVector::getWidth() {
	return log(this->qsv.getRows()) / log(2);
}

void StateVector::print() {
	this->qsv.print();
}

void StateVector::swapBits(int position1, int position2) {
	Matrix temp(1, this->qsv.getRows());
	int i;

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		if ( StateVector::isBitSet(i, position1) ^
			StateVector::isBitSet(i, position2) ) {
			int newPosition = i;
			if ( StateVector::isBitSet(i, position1) ) {
				newPosition -= pow(2, position1);
				newPosition += pow(2, position2);
			} else {
				newPosition -= pow(2, position2);
				newPosition += pow(2, position1);
			}
			temp.set(0, newPosition, this->qsv.get(0, i));
		} else {
			temp.set(0, i, this->qsv.get(0, i));
		}
	}
	this->qsv = temp;
}

void StateVector::reduce() {
	int i, j, isSetCount;
	double DOUBLE_ZERO = .000001;

	for ( i = 0; i < this->getWidth(); i++ ) {
		isSetCount = 0;
		for (j = 0; j < this->qsv.getRows(); j++ ) {
			if ( StateVector::isBitSet(j, i) ) {
				if ( abs(this->qsv.get(0, j)) > DOUBLE_ZERO ) {
					printf("bit %i set in %i\n", i, j);
					isSetCount++;
				}
			}
		}
		if ( isSetCount == 0 
			|| isSetCount == this->qsv.getRows() / 2 ) {
			printf("Get rid of bit at position %i\n", i);
		}
	}
}

bool StateVector::isBitSet(int index, int position) {
	return (int ( index / pow(2, position) ) % 2 == 1 );
}

}
