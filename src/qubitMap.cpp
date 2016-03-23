/*
 * qubitMap.cpp 
 */

#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "qubit.h"
#include "qubitMap.h"
#include "stateVector.h"

using namespace std;

namespace Quantum {

QubitMap* QubitMap::mapInstance = 0;

QubitMap::QubitMap() {
	this->qubitEntries = vector< shared_ptr<Qubit> >();
	this->mapIndex = 0;
	srand(time(NULL));
}

void QubitMap::addQubit(Qubit q) {
	this->addQubit(make_shared<Qubit>(q));
}

void QubitMap::addQubit(shared_ptr<Qubit> q) {
	this->qubitEntries.push_back(q);
}

int QubitMap::numQubits() {
	return this->qubitEntries.size();
}

shared_ptr<Qubit> QubitMap::getQubit(int i) {
	return this->qubitEntries.at(i);
}

int QubitMap::findQubit(int vectorIndex, int position) {
	int i;
	for ( i = 0; i < this->qubitEntries.size(); i++ ) {
		if ( this->qubitEntries.at(i)->position
			== position
			&& this->qubitEntries.at(i)->v->getIndex()
			== vectorIndex ) {
			break;
		}
	}
	return i;
}

void QubitMap::deleteQubit(int i) {
	this->qubitEntries.erase(this->qubitEntries.begin()+i);
}	

void QubitMap::addStateVector(StateVector v) {
	this->addStateVector(make_shared<StateVector>(v));
}

void QubitMap::addStateVector(shared_ptr<StateVector> v) {
	this->stateVectorEntries.push_back(v);
}

int QubitMap::numStateVectors() {
	return this->stateVectorEntries.size();
}

shared_ptr<StateVector> QubitMap::getStateVector(int i) {
	return this->stateVectorEntries.at(i);
}

void QubitMap::deleteStateVector(int i) {
	this->stateVectorEntries.erase(this->stateVectorEntries.begin()+i);
}

int QubitMap::getNewIndex() {
	return this->mapIndex++;
}

QubitMap* QubitMap::getInstance() {
	if ( QubitMap::mapInstance == 0 ) {
		QubitMap::mapInstance = new QubitMap();
	}
	return QubitMap::mapInstance;
}

}
