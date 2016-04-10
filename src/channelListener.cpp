/*
 * channelListener.cpp
 */
//STD C++ includes
#include <iostream>
#include <memory>
#include <string>
#include <thread>

//Google RPC includes
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/security/server_credentials.h>

//QooSim includes
#include "channel.h"
#include "channelListener.h"
#include "channelService.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
/*
 * getPort
 * @return the channel port
 * Get the channel port
 */
int ChannelListener::getPort() {
	return Channel::getServicePort();
}

/*
 * setPort
 * @param inPort the port number to set
 * Set the channel port
 */
void ChannelListener::setPort(int inPort) {
	Channel::setServicePort(inPort);
}

/*
 * Run
 * Start the channel service
 */
void ChannelListener::Run() {
	string serverAddr = "0.0.0.0:" + to_string(this->getPort());
	ChannelService service;
	grpc::ServerBuilder builder;

	builder.AddListeningPort(serverAddr, 
		grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	this->server = builder.BuildAndStart();
	this->server->Wait();
}

/*
 * Stop
 * Stop the channel service
 */
void ChannelListener::Stop() {
	this->server->Shutdown();
}
}
