#include <string>

#include "teleportClientRunnable.h"
#include "channelListener.h"
#include "qubit.h"
#include "system.h"

int main() {

	System* testSys = System::getInstance();
	shared_ptr<TeleportClientRunnable> crun ( new TeleportClientRunnable());

	crun->serverIP = "127.0.0.1";
	crun->serverPort = 50000;

	testSys->setAlgorithm(crun);
	testSys->setListenerPort(50100);

	testSys->RunSystem();
}
