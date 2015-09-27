/*
 * superposition_1.cpp
 */

#include "superposition_1_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50100);
	
	shared_ptr<iRunnable> a (new Superposition_1_Runnable() );
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
