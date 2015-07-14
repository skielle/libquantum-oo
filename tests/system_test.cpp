#include <stdio.h>
#include <math.h>
#include <memory>

#include "../gates.cpp"
#include "../matrix.h"
#include "../register.h"
#include "../system.h"
#include "../echoRunnable.h"

using namespace std;
using namespace Quantum;

int main() {
	System* mySys = System::getInstance();
	mySys->setListenerPort(50050);

	shared_ptr<iRunnable> a (new EchoRunnable() );

	mySys->setAlgorithm(a);
	mySys->RunSystem();
	
	return 0;
}
