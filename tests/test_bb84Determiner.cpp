#include "channelListener.h"
#include "qubit.h"
#include "system.h"
#include "bb84determiner_runnable.h"
#include "silentEvil.h"
#include "amplitudeDamping.h"

int main() {

	System* testSys = System::getInstance();
	shared_ptr<AmplitudeDamping> eve(new AmplitudeDamping());
	eve->setEta(.3);

	shared_ptr<BB84Determiner_Runnable> crun ( 
		new BB84Determiner_Runnable());

	testSys->setEvil(eve);
	testSys->setAlgorithm(crun);
	testSys->setListenerPort(50100);

	testSys->RunSystem();
}
