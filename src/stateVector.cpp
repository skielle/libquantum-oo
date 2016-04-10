/*
 * stateVector.cpp
 */
//STD C++ includes
#include <algorithm>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <vector>

//QooSim Includes
#include "channelService_client.h"
#include "matrix.h"
#include "qubit.h"
#include "qubitMap.h"
#include "remotePeer.h"
#include "stateVector.h"
#include "stateVectorOperation.h"

//External Includes
#include "externals/jacobi_eigenvalue.hpp"

using namespace std;

namespace Quantum {
/*
 * StateVector
 * Parameterless constructor, creates a StateVector of width 1
 */
StateVector::StateVector() : StateVector(1) {
}

/*
 * StateVector
 * @param bitWidth the width of the vector
 * Constructor, creates a vector with width equal to bitWidth
 */
StateVector::StateVector(int bitWidth) 
	: qsv(1, pow(2, bitWidth)), remoteQubits(bitWidth) {
	this->qsv.set(0, 0, 1);
}

/*
 * StateVector
 * @param m a matrix object
 * Constructor, creates a vector from the matrix object
 */
StateVector::StateVector(Matrix m) 
	: qsv(m), remoteQubits(log(m.getRows())/log(2)) {
}

/*
 * setIndex
 * @param newIndex the input index
 * Sets the memory map index of this vector
 */
void StateVector::setIndex(int newIndex) {
	this->index = newIndex;
}

/* 
 * getIndex
 * @return the memory map index of this vector
 */
int StateVector::getIndex() {
	return this->index;
}

/*
 * resize
 * @param newSize
 * Add space the the remoteQubits vector
 */
void StateVector::resize(int newSize) {
	this->remoteQubits.resize(newSize);
}

/*
 * applyOperation
 * @param m operation a matrix operator
 * @param input1 the position of the qubit to be operated on
 * Shortcut method to apply an operation to one qubit already in
 * this vector
 */
void StateVector::applyOperation(Matrix operation, int input1) {
	vector<int> inputs(1);
	inputs.at(0) = input1;
	this->applyOperation(operation, inputs);
}

/*
 * applyOperation
 * @param m operation a matrix operator
 * @param input1 the position of a qubit to be operated on
 * @param input2 the position of a qubit to be operated on
 * Shortcut method to apply an operation to two qubits already in
 * this vector
 */
void StateVector::applyOperation(Matrix operation, int input1, int input2) {
	vector<int> inputs(2);
	inputs.at(0) = input1;
	inputs.at(1) = input2;
	this->applyOperation(operation, inputs);
}

/*
 * applyOperation
 * @param m operation a matrix operator
 * @param inputs the positions of qubits to be operated on
 * Apply an operation to an arbitrary number of qubits in this vector
 */
void StateVector::applyOperation(Matrix operation, vector<int> inputs) {
	this->applyOperation(operation, inputs, true);
}

/*
 * applyOperation
 * @param operation a matrix operator
 * @param inputs the positions of qubits to be operated on
 * @param addToHistory whether or not to push this operation onto the
 * vector's history
 * Apply an operation to an arbitrary number of qubits in this vector
 */
void StateVector::applyOperation(Matrix operation, vector<int> inputs,
	bool addToHistory) {
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

	if ( addToHistory ) {
		this->reduce();
		this->opHistory.push_back(
			StateVectorOperation(operation, inputs));
	}
}

/*
 * applyOperation
 * @param operation a matrix operator
 * @param inputs the qubit objects on which to apply the operation
 * Apply an operation to an arbitrary number of qubits, consolidating them
 * into this vector first
 */
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

/*
 * generateRowMap
 * @param inputs the inputs to generate a map for
 * @return integer vector of the rowmap
 * Generate a mapping of the rows in this vector in order to place the 
 * inputs provided in the left most positions
 */
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

/*
 * getWidth
 * @returns the bit width of this vector
 */
int StateVector::getWidth() {
	return log(this->qsv.getRows()) / log(2);
}

/*
 * print
 * Prints this vector
 */
void StateVector::print() {
	printf("Index: %i\r\n", this->index);
	this->qsv.print();
}

/*
 * reduce
 * Removed unentanged qubits from this vector, splitting them off
 * into their own vectors
 */
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

/*
 * isBitSet
 * @param index The index in the state vector
 * @param position The bit position
 * @return bool whether or not the bit position is set to zero or one
 */
bool StateVector::isBitSet(int index, int position) {
	return (int ( index / pow(2, (position)) ) 
			% 2 == 1 );
}

/* 
 * getAlpha
 * @param position
 * @returns alpha value for this bit
 * Determine the overall alpha (0) probability amplitude of the 
 * bit position in this vector
 */
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

/* 
 * getBeta
 * @param position
 * @returns beta value for this bit
 * Determine the overall beta (1) probability amplitude of the 
 * bit position in this vector
 */
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

/*
 * measure
 * @param position the bit to measure
 * @return the measured value
 * Perform a pseudo random measurement of the bit given
 */
int StateVector::measure(int position) {
	int value = 0;
	double measurement = rand() / (float)RAND_MAX;

	if ( this->getAlpha(position) < measurement ) {
		value = 1;
	}

	return this->measure(position, value);
}

/*
 * measure
 * @param position the bit to measure
 * @param forceResult
 * @return the measured value
 * Perform a forced measurement of the bit given
 */
int StateVector::measure(int position, int forceResult) {
	this->measure(position, forceResult, true);
}

/*
 * measure
 * @param position the bit to measure
 * @param forceResult
 * @param propagate
 * @return the measured value
 * Perform a forced measurement of the bit given, 
 * propagating the measuredment to any remote peers
 */
int StateVector::measure(int position, int forceResult, 
	bool propagate) {
	int i;
	vector<int> peersNotified;
	Matrix initialState(this->qsv);

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

/*
 * fromDensity
 * @param rho the density matrix
 * Convert a density matrix to a state vector
 */
void StateVector::fromDensity(Matrix rho) {
	int i, j;
	int N = rho.getRows();
	double A[N*N];
	double V[N*N];
	double D[N];
	int it_num;
	int rot_num;

	if ( rho.getRows() != rho.getCols() 
		|| rho.getRows() != this->qsv.getRows() ) {
		return;
	}

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		this->qsv.set(0, i, 0);
	}

	for ( i = 0; i < N; i++ ) {
		for ( j = 0; j < N; j++ ) {
			A[i*N + j] = real(rho.get(i, j));
		}
	}
	jacobi_eigenvalue(N, A, 20, V, D, it_num, rot_num);
	for ( i = 0; i < N; i++ ) {
		for ( j = 0; j < N; j++ ) {
			complex<double> temp = this->qsv.get(0, j);
			temp += D[i] * V[i*N+j];
			this->qsv.set(0, j, temp);
		}
	}
}

/*
 * toDensity
 * @return the density matrix
 * Convert this state vector to a density matrix
 */
Matrix StateVector::toDensity() {
	int i, j;
	Matrix rho(this->qsv.getRows(), this->qsv.getRows());

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		for ( j = 0; j < this->qsv.getRows(); j++ ) {
			complex<double> temp = rho.get(i, j);
			temp += this->qsv.get(i, 0) * this->qsv.get(j, 0);
			rho.set(i, j, temp);
		}		
	}
	return rho;
}

/*
 * normalize
 * Normalize probability amplitudes across the state vector
 */
void StateVector::normalize() {
	int i;
	double realPart = 0.0;
	double imagPart = 0.0;

	double total = 0.0;

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		realPart = real(this->qsv.get(0, i));
		imagPart = imag(this->qsv.get(0, i));
		total += realPart*realPart + imagPart*imagPart;
	}

	for ( i = 0; i < this->qsv.getRows(); i++ ) {
		realPart = real(this->qsv.get(0, i));
		imagPart = imag(this->qsv.get(0, i));

		this->qsv.set(0, i, 
			sqrt((realPart*realPart + imagPart*imagPart) / total));
	}
}

void sync() {

}

/*
 * toMatrix
 * @return this vector as a matrix
 */
Matrix StateVector::toMatrix() {
	return this->qsv;
}

/*
 * replay
 * Replay this state vector's history
 */
void StateVector::replay() {
	int i = 0;

	for ( i = 0; i < this->opHistory.size(); i++ ) {
		this->applyOperation(
			this->opHistory.at(i).getOperation(),
			this->opHistory.at(i).getArgs(),
			false
		);
	}
	this->opHistory.clear();
}

}
