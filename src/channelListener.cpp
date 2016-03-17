/*
 * channelListener.cpp
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/security/server_credentials.h>
#include "channelListener.h"
#include "channelService.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
int ChannelListener::getPort() {
	return this->port;
}

void ChannelListener::setPort(int inPort) {
	this->port = inPort;
}

void ChannelListener::Run() {
	string serverAddr = "0.0.0.0:" + to_string(this->port);
	ChannelService service;
	service.setServicePort(this->port);
	grpc::ServerBuilder builder;

	builder.AddListeningPort(serverAddr, 
		grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	this->server = builder.BuildAndStart();
	cout<<"Server running on "<<serverAddr<<endl;
	this->server->Wait();
}

void ChannelListener::Stop() {
	this->server->Shutdown();
}
}
