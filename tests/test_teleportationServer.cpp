#include "channelListener.h"
#include "qubit.h"
#include "system.h"
#include "silentEvil.h"
#include "teleportServerRunnable.h"

int main() {
	System* testSys = System::getInstance();
	shared_ptr<TeleportServerRunnable> crun ( new TeleportServerRunnable());
	shared_ptr<SilentEvil> eve(new SilentEvil());
	eve->setEvilness(0);

	testSys->setAlgorithm(crun);
	testSys->setEvil(eve);
	testSys->setListenerPort(50000);

	testSys->RunSystem();
}
