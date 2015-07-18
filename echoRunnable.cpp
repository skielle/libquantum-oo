/*
 * echoRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <utility>
#include <stdio.h>

#include "classicRegister.h"
#include "echoRunnable.h"
#include "register.h"
#include "system.h"
#include "systemMessage.h"

using namespace std;

namespace Quantum {
void EchoRunnable::Run() {
	System* sys = System::getInstance();

	printf("ECHORUNNABLE\n");

	sleep(5);
	while ( true ) {
		sleep(1);
		if ( !sys->isMessageQueueEmpty() ) {
			if ( sys->getMessageType() ==
				SystemMessage::REGISTER_RECIEVED ) {
				shared_ptr<Register> rx 
					= sys->getMessage<Register>();
			rx->print();
			}
			if ( sys->getMessageType() ==
				SystemMessage::CLASSIC_REGISTER_RECIEVED ) {
				shared_ptr<ClassicRegister> rx
					= sys->getMessage<ClassicRegister>();
			rx->print();
			}
		}
	}
}
}
