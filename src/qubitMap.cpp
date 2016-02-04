/*
 * qubitMap.cpp 
 */

#include <memory>
#include <vector>

#include "qubitMap.h"
#include "qubitMapEntry.h"
#include "stateVector.h"

using namespace std;

namespace Quantum {

QubitMap* QubitMap::mapInstance = 0;

QubitMap::QubitMap() {
	this->mapEntries = vector< shared_ptr<QubitMapEntry> >();
}

QubitMap* QubitMap::getInstance() {
	if ( QubitMap::mapInstance == 0 ) {
		QubitMap::mapInstance = new QubitMap();
	}
	return QubitMap::mapInstance;
}

shared_ptr<QubitMapEntry> QubitMap::createQubit() {
	shared_ptr<QubitMapEntry> m (new QubitMapEntry);
	m->mapVector = make_shared<StateVector>(1);
	m->position = 1;

	QubitMap::getInstance()->mapEntries.push_back(m);
	return QubitMap::getInstance()->mapEntries.back();
}

}
