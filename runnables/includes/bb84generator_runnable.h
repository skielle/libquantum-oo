/*
 * bb84generator_runnable.h
 */

#ifndef __QUANTUM_BB84_GENERATOR_RUNNABLE_H
#define __QUANTUM_BB84_GENERATOR_RUNNABLE_H

#include <string>
#include "iRunnable.h"

using namespace std;
using namespace Quantum;

namespace Quantum {
static int BB84_BURST_SIZE = 128;
static int BB84_KEY_SIZE = 512;
static int BB84_RAW_SIZE = 1024;

class BB84Generator_Runnable : public iRunnable {
	public:
		string serverIP = "127.0.0.1";
		int serverPort = 50100;
		virtual void Run();
};
}
#endif
