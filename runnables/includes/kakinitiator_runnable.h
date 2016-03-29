/*
 * kakinitiator_runnable.h
 */

#ifndef __QUANTUM_KAK_INITIATOR_RUNNABLE_H
#define __QUANTUM_KAK_INITIATOR_RUNNABLE_H

#include <string>
#include "iRunnable.h"

using namespace std;
using namespace Quantum;

namespace Quantum {

class KakInitiator_Runnable : public iRunnable {
	public:
		string serverIP = "127.0.0.1";
		int serverPort = 50100;
		virtual void Run();
};
}

#endif
