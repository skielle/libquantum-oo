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

	while ( sys->isMessageQueueEmpty() ) {
	}

	if ( sys->nextMessageType()
		== SystemMessage::QUANTUM_DATA_RECEIVED ) {
		int address = sys->nextMessage();

		shared_ptr<Qubit> q = qm->getQubit(i);

		for ( i = 0; i < 3; i++ ) {
			q->print();
			sleep(10);
		}
	}

	sys->stopServer();
}
}
