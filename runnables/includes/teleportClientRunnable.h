/*
 * teleportClientRunnable.h
 */
#ifndef __QUANTUM_TELEPORTCLIENTRUNNABLE_H
#define __QUANTUM_TELEPORTCLIENTRUNNABLE_H

#include <string>

#include "iRunnable.h"

using namespace std;

namespace Quantum {
class TeleportClientRunnable : public iRunnable {
	public:
		string serverIP = "127.0.0.1";
		int serverPort = 50100;
		virtual void Run(); 
};
}
#endif
