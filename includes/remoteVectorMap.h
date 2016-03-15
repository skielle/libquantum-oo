/*
 * remoteVectorMap.h
 */

#ifndef __QUANTUM_REMOTE_VECTOR_MAP_H
#define __QUANTUM_REMOTE_VECTOR_MAP_H

#include <memory>
#include <vector>
#include <string>

#include "stateVector.h"

using namespace std;

namespace Quantum {
class RemoteVectorMapEntry {
	public:
		string remoteSystem;
		int remoteIndex;
		int localIndex;
};

class RemoteVectorMap {
	private:
		RemoteVectorMap();
		static RemoteVectorMap* mapInstance;
	public:
		vector<RemoteVectorMapEntry> mapEntries;

		int getLocalIndex(string remoteSystem, int remoteIndex);
		
		static RemoteVectorMap* getInstance();
};
}
#endif
