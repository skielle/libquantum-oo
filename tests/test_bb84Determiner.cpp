#include "channelListener.h"
#include "qubit.h"
#include "system.h"
#include "bb84determiner_runnable.h"
#include "silentEvil.h"
#include "silentEvil_AD.h"
#include "amplitudeDamping.h"
#include "rotationError.h"

int main(int argc, char* argv[]) {

	System* testSys = System::getInstance();
	shared_ptr<SilentEvil_AD> eve(new SilentEvil_AD());
	eve->setEvilness(1);
	eve->setEta((float)atoi(argv[1])/100);

	shared_ptr<BB84Determiner_Runnable> crun ( 
		new BB84Determiner_Runnable());

	testSys->setEvil(eve);
	testSys->setAlgorithm(crun);
	testSys->setListenerPort(50100);

	testSys->RunSystem();
}
