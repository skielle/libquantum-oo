/*
 * kakRequestor.cpp
 */

#include "kakParty_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50106);

	KAKParty_Runnable* ka = new KAKParty_Runnable();
	ka->setInstigator(true);	
	
	shared_ptr<iRunnable> a (ka);
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
