#include "channelListener.h"
#include "qubit.h"
#include "system.h"
#include "kakresponder_runnable.h"
#include "silentEvil.h"
#include "amplitudeDamping.h"

int main() {

	System* testSys = System::getInstance();
	shared_ptr<AmplitudeDamping> eve(new AmplitudeDamping());
	eve->setEta(.99);

	shared_ptr<KakResponder_Runnable> crun ( 
		new KakResponder_Runnable());

	testSys->setEvil(eve);
	testSys->setAlgorithm(crun);
	testSys->setListenerPort(50100);

	testSys->RunSystem();
}
