/*
 * remotePeer.h
 */

#ifndef __QUANTUM_REMOTE_PEER_H
#define __QUANTUM_REMOTE_PEER_H

#include <memory>
#include <vector>
#include <string>

using namespace std;

namespace Quantum {
class RemotePeerEntry {
	public:
		string peerIP = "";
		int peerClientPort = 0;
		int peerServicePort = 0;
		int peerPID = 0;
};

class RemotePeerList {
	private:
		RemotePeerList();
		static RemotePeerList* peerListInstance;
	public:
		vector<RemotePeerEntry> peerList;

		static RemotePeerList* getInstance();
		
		int lookupPeerByPID(string ip, int pid);
		int lookupPeerByServicePort(string ip, int port);
		int lookupPeerByClientPort(string ip, int port);
};
}
#endif
