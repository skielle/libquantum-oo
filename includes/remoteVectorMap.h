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
		int localIndex;
		int position;
		string remoteSystem;
		int remoteIndex;
};

class RemoteVectorMap {
	private:
		RemoteVectorMap();
		static RemoteVectorMap* mapInstance;
	public:
		vector<RemoteVectorMapEntry> mapEntries;

		int getLocalIndex(string remoteSystem, int remoteIndex);
		int getMapEntryIfRemote(int localIndex, int position);
		static RemoteVectorMap* getInstance();
};
}
#endif
