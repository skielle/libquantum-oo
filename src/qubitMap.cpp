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
	this->mapEntries = vector< shared_ptr<Qubit> >();
	this->mapIndex = 0;
	srand(time(NULL));
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
