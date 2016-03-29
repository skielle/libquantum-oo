#include <string>

#include "kakinitiator_runnable.h"
#include "channelListener.h"
#include "qubit.h"
#include "system.h"
#include "amplitudeDamping.h"

int main() {

	System* testSys = System::getInstance();
	shared_ptr<KakInitiator_Runnable> crun ( 
		new KakInitiator_Runnable());
	shared_ptr<AmplitudeDamping> eve(new AmplitudeDamping());
	eve->setEta(.99);

	crun->serverIP = "127.0.0.1";
	crun->serverPort = 50100;

	testSys->setAlgorithm(crun);
	testSys->setListenerPort(50110);

	testSys->RunSystem();
}
