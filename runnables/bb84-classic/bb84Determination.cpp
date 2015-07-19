/*
 * bb84Determination.cpp
 */

#include "bb84Determination_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50085);
	
	shared_ptr<iRunnable> a (new BB84Determination_Runnable() );
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
