/*
 * controllerListener.cpp
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/security/server_credentials.h>
#include "controllerListener.h"
#include "controllerListenerService.h"

using namespace std;
using namespace QuantumGUI;

namespace QuantumGUI {
int ControllerListener::getPort() {
	return this->port;
}

void ControllerListener::setPort(int inPort) {
	this->port = inPort;
}

void ControllerListener::setController(shared_ptr<iController> inController) {
	this->controller = inController;
}

void ControllerListener::Run() {
	string serverAddr = "0.0.0.0:" + to_string(this->port);
	shared_ptr<ControllerListenerService> 
		service (new ControllerListenerService);

	grpc::ServerBuilder builder;

	builder.AddListeningPort(serverAddr, 
		grpc::InsecureServerCredentials());
	builder.RegisterService(&(*service.get()));
	this->server = builder.BuildAndStart();
	service->setController(this->controller);
	this->controller->setService(service);
	cout<<"Server running on "<<serverAddr<<endl;
	this->server->Wait();
}

void ControllerListener::Stop() {
	this->server->Shutdown();
}
}
