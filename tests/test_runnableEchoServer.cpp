#include "channelListener.h"
#include "qubit.h"
#include "system.h"

int main() {

	System* testSys = System::getInstance();

	testSys->setListenerPort(50000);

	testSys->RunSystem();
}
