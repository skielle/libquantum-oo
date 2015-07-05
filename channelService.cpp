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
	s->addRegister(rg);
	return grpc::Status::OK;
}
}
