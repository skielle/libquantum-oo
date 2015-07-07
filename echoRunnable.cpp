/*
 * echoRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <utility>
#include <stdio.h>
#include "echoRunnable.h"
#include "register.h"
#include "system.h"

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
				sys->getMessage<Register>();
				//Register rx = this->getMessage<Register>();
				//rx.print();
			}
		}
	}
}
}
