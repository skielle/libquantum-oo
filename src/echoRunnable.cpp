/*
 * echoRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <utility>
#include <stdio.h>

#include "echoRunnable.h"
#include "system.h"

using namespace std;

namespace Quantum {
void EchoRunnable::Run() {
	System* sys = System::getInstance();

	printf("ECHORUNNABLE\n");

	sleep(5);
	while ( true ) {
		sleep(1);
		printf("RUNNING RUNNABLE...");
	}
}
}
