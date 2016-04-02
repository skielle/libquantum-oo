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
#include "gates.h"

using namespace std;

namespace Quantum {
void EchoRunnable::Run() {
	System* sys = System::getInstance();

	int i;
	RemotePeerList* rpl = RemotePeerList::getInstance();	
	QubitMap* qm = QubitMap::getInstance();
	
	while ( sys->isMessageQueueEmpty() ) {
	}

	if ( sys->nextMessageType()
		== SystemMessage::QUANTUM_DATA_RECEIVED ) {
		int address = sys->nextMessage();

		sleep(1);
		shared_ptr<Qubit> q2 = qm->getQubit(i);
		sleep(5);
		q2->print();
	}

	sys->stopServer();
}
}
