/*
 * source.cpp
 */

#include "source_runnable.h"
#include "system.h"

int main() {
	System* sys = System::getInstance();

	sys->setListenerPort(50101);

	shared_ptr<iRunnable> a(new Source_Runnable());
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
