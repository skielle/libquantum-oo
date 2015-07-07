/*
 * channelService.cpp
 */

#include <memory>
#include <thread>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/status.h>
#include "channelService.h"
#include "classicRegister.h"
#include "register.h"
#include "system.h"
#include "quantumMessage.grpc.pb.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
grpc::Status ChannelService::SendRegister(grpc::ServerContext* context, 
	const QuantumMessage::RegisterMessage* request, 
	QuantumMessage::VoidMessage* reply) {
	System* s = System::getInstance();
	shared_ptr<Register> rg (&Register::unserialize(request));
	s->addRegister(rg, SystemMessage::REGISTER_RECIEVED);
	return grpc::Status::OK;
}
grpc::Status ChannelService::SendClassicRegister(grpc::ServerContext* context, 
	const QuantumMessage::ClassicRegisterMessage* request, 
	QuantumMessage::VoidMessage* reply) {
	System* s = System::getInstance();
	shared_ptr<ClassicRegister> rg (&ClassicRegister::unserialize(request));
	s->addRegister(rg, SystemMessage::CLASSIC_REGISTER_RECIEVED);
	return grpc::Status::OK;
}
}
