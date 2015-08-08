/*
 * sink.cpp
 */

#include "sink_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50100);
	
	shared_ptr<iRunnable> a (new Sink_Runnable() );
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
