/*
 * kakFulfiller.cpp
 */

#include "kakParty_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50107);
	
	shared_ptr<iRunnable> a (new KAKParty_Runnable() );
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
