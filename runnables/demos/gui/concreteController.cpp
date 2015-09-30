/*
 * concreteController.cpp
 */

#include <google/protobuf/message.h>
#include <queue>
#include <string>
#include <utility>

#include "controllerListener.h"
#include "concreteController.h"
#include "iController.h"

namespace QuantumGUI {
shared_ptr<ConcreteController> ConcreteController::create() {
	return shared_ptr<ConcreteController>(new ConcreteController);
}

void ConcreteController::Run() {
	ControllerListener cl = ControllerListener();
	cl.setController(shared_from_this());
	cl.setPort(60000);
	cl.Run();
}

void ConcreteController::Process() {
	pair<string, const google::protobuf::Message*> regm = 
		this->service->requestQueue.front();
	this->service->requestQueue.pop();
	printf("Got message type: %s\r\n", regm.first.c_str());
}
}
