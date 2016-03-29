/*
 * kakresponder_runnable.h
 */

#ifndef __QUANTUM_KAK_RESPONDER_RUNNABLE_H
#define __QUANTUM_KAK_RESPONDER_RUNNABLE_H

#include <string>
#include "iRunnable.h"

using namespace std;
using namespace Quantum;

namespace Quantum {
static int KAK_KEY_SIZE = 1024;
static int KAK_BURST_SIZE = 256;

class KakResponder_Runnable : public iRunnable {
	public:
		string serverIP = "127.0.0.1";
		int serverPort = 50100;
		virtual void Run();
};
}
#endif
