/*
 * echoClientRunnable.h
 */
#ifndef __QUANTUM_ECHOCLIENTRUNNABLE_H
#define __QUANTUM_ECHOCLIENTRUNNABLE_H

#include <string>

#include "iRunnable.h"

using namespace std;

namespace Quantum {
class EchoClientRunnable : public iRunnable {
	public:
		string serverIP = "127.0.0.1";
		int serverPort = 50100;
		virtual void Run(); 
};
}
#endif
