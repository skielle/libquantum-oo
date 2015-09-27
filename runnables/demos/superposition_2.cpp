/*
 * superposition_2.cpp
 */

#include "superposition_2_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50100);
	
	shared_ptr<iRunnable> a (new Superposition_2_Runnable() );
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
