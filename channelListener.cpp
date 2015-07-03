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
#include <grpc++/server_credentials.h>
#include "channelListener.h"
#include "channelService.h"
#include "system.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
void ChannelListener::Run(int port) {
	string serverAddr = "0.0.0.0:" + to_string(port);
	ChannelService service;
	grpc::ServerBuilder builder;

	builder.AddListeningPort(serverAddr, 
		grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	unique_ptr<grpc::Server> server(builder.BuildAndStart());
	cout<<"Server running on "<<serverAddr<<endl;
	server->Wait();
}
}
