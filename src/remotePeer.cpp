/*
 * remotePeer.cpp 
 */

#include <memory>
#include <vector>
#include <string>

#include "remotePeer.h"

using namespace std;

namespace Quantum {

RemotePeerList* RemotePeerList::peerListInstance = 0;

RemotePeerList::RemotePeerList() {
	this->peerList = vector<RemotePeerEntry>();
}

RemotePeerList* RemotePeerList::getInstance() {
	if ( RemotePeerList::peerListInstance == 0 ) {
		RemotePeerList::peerListInstance = new RemotePeerList();
	}
	return RemotePeerList::peerListInstance;
}

int RemotePeerList::lookupPeerByPID(string ip, int pid) {
	int i;

	for ( i = 0; i < this->peerList.size(); i++ ) {
		if ( this->peerList.at(i).peerIP == ip &&
			this->peerList.at(i).peerPID == pid ) {
			return i;
		}
	}

	if ( i == this->peerList.size() ) {
		RemotePeerEntry rpe;
		rpe.peerIP = ip;
		rpe.peerPID = pid;
		this->peerList.push_back(rpe);
		return i;
	}
}

int RemotePeerList::lookupPeerByServicePort(string ip, int port) {
	int i;

	for ( i = 0; i < this->peerList.size(); i++ ) {
		if ( this->peerList.at(i).peerIP == ip &&
			this->peerList.at(i).peerServicePort == port ) {
			return i;
		}
	}

	if ( i == this->peerList.size() ) {
		RemotePeerEntry rpe;
		rpe.peerIP = ip;
		rpe.peerServicePort = port;
		this->peerList.push_back(rpe);
		return i;
	}
}

}
