/*
 * echoClientRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <utility>
#include <stdio.h>
#include <string>

#include "echoClientRunnable.h"
#include "system.h"
#include "remotePeer.h"
#include "qubit.h"
#include "qubitMap.h"
#include "channelService_client.h"

using namespace std;

namespace Quantum {
void EchoClientRunnable::Run() {
	int i;

	QuantumChannel::ChannelService_client csc(this->serverIP, this->serverPort);
	printf("ECHO CLIENT RUNNABLE\n");

	sleep(5);
	while ( true ) {
		sleep(10);
		shared_ptr<Qubit> q = Qubit::create();

		csc.SendCallbackPort();
		csc.SendQubit(q);	
	}

}
}
