#include <string>

#include "bb84generator_runnable.h"
#include "channelListener.h"
#include "qubit.h"
#include "system.h"

int main() {

	System* testSys = System::getInstance();
	shared_ptr<BB84Generator_Runnable> crun ( 
		new BB84Generator_Runnable());

	crun->serverIP = "127.0.0.1";
	crun->serverPort = 50100;

	testSys->setAlgorithm(crun);
	testSys->setListenerPort(50110);

	testSys->RunSystem();
}
