/*
 * modelListener.h
 */

#ifndef __QUANTUMGUI_MODEL_LISTENER_H
#define __QUANTUMGUI_MODEL_LISTENER_H

#include <grpc++/server.h>
#include <memory>
#include "iRunnable.h"

using namespace Quantum;
using namespace std;

namespace QuantumGUI {
class ModelListener : public iRunnable {
	private:
		int port;
		unique_ptr<grpc::Server> server;
	public:
		int getPort();
		void setPort(int inPort);
		void Run();
		void Stop();
		
};
}

#endif
