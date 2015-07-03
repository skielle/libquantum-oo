/*
 * channelListener_test.cpp
 */

#include "../channelListener.h"
#include "../register.h"
#include "../system.h"

using namespace std;
using namespace Quantum;
using namespace QuantumChannel;

int main () {
	ChannelListener cl;
	cl.Run(50051);

	return 0;
}
