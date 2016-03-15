/*
 * remoteVectorMap.cpp 
 */

#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "qubit.h"
#include "qubitMap.h"
#include "remoteVectorMap.h"
#include "stateVector.h"

using namespace std;

namespace Quantum {

RemoteVectorMap* RemoteVectorMap::mapInstance = 0;

RemoteVectorMap::RemoteVectorMap() {
	this->mapEntries = vector<RemoteVectorMapEntry>();
	srand(time(NULL));
}

int RemoteVectorMap::getLocalIndex(string remoteSystem, int remoteIndex) {
	int i = 0;

	for ( i = 0; i < this->mapEntries.size(); i++ ) {
		if ( this->mapEntries.at(i).remoteSystem == remoteSystem
			&& this->mapEntries.at(i).remoteIndex == remoteIndex ) {
			return this->mapEntries.at(i).localIndex;
		}
	}

	i = QubitMap::getInstance()->getNewIndex();

	RemoteVectorMapEntry r;
	r.localIndex = i;
	r.remoteIndex = remoteIndex;
	r.remoteSystem = remoteSystem;

	this->mapEntries.push_back(r);

	return i;
}
	
RemoteVectorMap* RemoteVectorMap::getInstance() {
	if ( RemoteVectorMap::mapInstance == 0 ) {
		RemoteVectorMap::mapInstance = new RemoteVectorMap();
	}
	return RemoteVectorMap::mapInstance;
}

}
