/*
 * controllerListener.h
 */

#ifndef __QUANTUMGUI_CONTROLLER_LISTENER_H
#define __QUANTUMGUI_CONTROLLER_LISTENER_H

#include <grpc++/server.h>
#include <memory>

#include "iController.h"

using namespace std;

namespace QuantumGUI {
class ControllerListener {
	private:
		shared_ptr<iController> controller;
		int port;
		unique_ptr<grpc::Server> server;
	public:
		int getPort();
		void setPort(int inPort);
		void setController(shared_ptr<iController> controller);
		void Run();
		void Stop();
		
};
}

#endif
