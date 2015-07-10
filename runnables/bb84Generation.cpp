/*
 * bb84Generation.cpp
 */

#include "bb84Generation_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50084);
	
	shared_ptr<iRunnable> a (new BB84Generation_Runnable() );
	sys->setAlgorithm(a);

	sys->runServer();	

	return 0;
}
