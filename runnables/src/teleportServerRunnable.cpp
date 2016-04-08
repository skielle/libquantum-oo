/*
 * teleportServerRunnable.cpp
 */
//STD C++ includes
#include <memory>
#include <unistd.h>
#include <utility>
#include <stdio.h>

//Qoosim includes
#include "gates.h"
#include "qubit.h"
#include "qubitMap.h"
#include "system.h"

//runnable header
#include "teleportServerRunnable.h"

using namespace std;
namespace Quantum {
/*
 * Run
 * @param none
 * @return none
 */
void TeleportServerRunnable::Run() {
	//Get the system object
	System* sys = System::getInstance();

	//Get the memory map object
	QubitMap* qm = QubitMap::getInstance();

	//Wait for a message to come in	
	while ( sys->isMessageQueueEmpty() ) {
	}

	//If the message is a quantum message, we assume it's our half of q1q2
	if ( sys->nextMessageType()
		== SystemMessage::QUANTUM_DATA_RECEIVED ) {
		//Get the address of the received qubit
		int address = sys->nextMessage();
		//Retrieve the qubit from the address
		shared_ptr<Qubit> q2 = qm->getQubit(address);

		//Wait 5 seconds for the client to do her operations
		sleep(5);

		//Violate the laws of physics and print the state of the qubit
		q2->print();
	}

	sys->stopServer();
}
}
