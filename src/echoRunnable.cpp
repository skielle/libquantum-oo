/*
 * echoRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <utility>
#include <stdio.h>

#include "echoRunnable.h"
#include "system.h"
#include "remotePeer.h"
#include "qubit.h"
#include "qubitMap.h"

using namespace std;

namespace Quantum {
void EchoRunnable::Run() {
	System* sys = System::getInstance();

	int i;
	RemotePeerList* rpl = RemotePeerList::getInstance();	
	QubitMap* qm = QubitMap::getInstance();

	printf("ECHORUNNABLE\n");

	sleep(5);
	while ( true ) {
		sleep(10);
		printf("RUNNING RUNNABLE...\r\n");
		printf("Peer List (%i): \r\n", rpl->peerList.size());
		for ( i = 0; i < rpl->peerList.size(); i++ ) {
			printf("Peer PID: %i\r\n", rpl->peerList.at(i).peerPID);
			printf("Peer IP: %s\r\n", rpl->peerList.at(i).peerIP.c_str());
			printf("Peer Service Port: %i\r\n", rpl->peerList.at(i).peerServicePort);
		}
		printf("Qubit List: \r\n");
		for ( i = 0; i < qm->mapEntries.size(); i++ ) {
			printf("Qubit: %i in vector %i\r\n", qm->mapEntries.at(i)->position, qm->mapEntries.at(i)->v->getIndex());
			qm->mapEntries.at(i)->print();
		}
	}
}
}
