#include "channelListener.h"
#include "qubit.h"
#include "system.h"
#include "kakresponder_runnable.h"
#include "silentEvil.h"
#include "silentEvil_AD.h"
#include "amplitudeDamping.h"
#include "rotationError.h"

int main(int argc, char* argv[]) {
	System* testSys = System::getInstance();
	shared_ptr<RotationError> eve(new RotationError());
	eve->setTheta((float)atoi(argv[1])/90*M_PI/2);

	shared_ptr<KakResponder_Runnable> crun ( 
		new KakResponder_Runnable());

	testSys->setEvil(eve);
	testSys->setAlgorithm(crun);
	testSys->setListenerPort(50100);

	testSys->RunSystem();
}
