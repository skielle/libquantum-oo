#include <stdio.h>
#include <math.h>
#include "../gates.cpp"
#include "../matrix.h"
#include "../register.h"
#include "../system.h"

using namespace std;
using namespace Quantum;

int main() {
	System* mySys = System::getInstance();
	mySys->setListenerPort(50050);
	mySys->runServer();
	
	return 0;
}
