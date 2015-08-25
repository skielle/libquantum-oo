/*
 * bb84Determination_runnable.cpp
 */
#include <string>
#include <unistd.h>

#include "sink_runnable.h"
#include "channelListener.h"
#include "channelService_client.h"
#include "classicRegister.h"
#include "entangledPair.h"
#include "entangledRegister.h"
#include "entanglement.h"
#include "gates.cpp"
#include "register.h"
#include "system.h"
#include "systemMessage.h"

using namespace std;
using namespace Quantum;

void Sink_Runnable::Run() {
	System* sys = System::getInstance();

	shared_ptr<ClassicRegister> r;

	sys->addRegister(r, SystemMessage::CLASSIC_REGISTER_RECIEVED);

	QuantumChannel::ChannelService_client csc("127.0.0.1", 50084);
	while (true) {
		sleep(1);
	while ( sys->isMessageQueueEmpty() ) {
		sleep(1);
		printf("Nothing  happened...\n");
	}
	if ( sys->getMessageType() == 
		SystemMessage::ENTANGLED_REGISTER_RECIEVED ) {
		EntangledRegister quBitReg = *(sys->getMessage<EntangledRegister>());
//		quBitReg.measure(1, true);
		quBitReg.print();
		printf("Something  happened...\n");
	} else {
		printf("Something else happened...%i\n", sys->getMessageType());
		sys->getMessage<ClassicRegister>();
	}
	}

	//sys->stopServer();
}
