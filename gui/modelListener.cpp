/*
 * modelListener.cpp
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_credentials.h>
#include "modelListener.h"
#include "modelListenerService.h"

using namespace std;
using namespace Quantum;

namespace QuantumGUI {
int ModelListener::getPort() {
	return this->port;
}

void ModelListener::setPort(int inPort) {
	this->port = inPort;
}

void ModelListener::Run() {
	string serverAddr = "0.0.0.0:" + to_string(this->port);
	ModelListenerService service;
	grpc::ServerBuilder builder;

	builder.AddListeningPort(serverAddr, 
		grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	this->server = builder.BuildAndStart();
	cout<<"Server running on "<<serverAddr<<endl;
	this->server->Wait();
}

void ModelListener::Stop() {
	this->server->Shutdown();
}
}
