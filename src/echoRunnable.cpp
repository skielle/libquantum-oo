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
		for ( i = 0; i < qm->numQubits(); i++ ) {
			shared_ptr<Qubit> q = qm->getQubit(i);
			printf("Qubit: %i in vector %i\r\n", q->position, q->v->getIndex());
			q->print();
		}
	}
}
}
