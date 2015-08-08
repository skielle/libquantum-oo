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
#include "entangledRegister.h"
#include "register.h"
#include "system.h"
#include "systemMessage.h"
#include "quantumMessage.grpc.pb.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
grpc::Status ChannelService::SendRegister(grpc::ServerContext* context, 
	const QuantumMessage::RegisterMessage* request, 
	QuantumMessage::VoidMessage* reply) {
	System* sys = System::getInstance();
	shared_ptr<Register> rg (&Register::unserialize(request));
	sys->eve->doEvil(rg, SystemMessage::REGISTER_RECIEVED);
	sys->addRegister(rg, SystemMessage::REGISTER_RECIEVED);
	return grpc::Status::OK;
}
grpc::Status ChannelService::SendClassicRegister(grpc::ServerContext* context, 
	const QuantumMessage::ClassicRegisterMessage* request, 
	QuantumMessage::VoidMessage* reply) {
	System* sys = System::getInstance();
	shared_ptr<ClassicRegister> rg (&ClassicRegister::unserialize(request));
	sys->eve->doEvil(rg, SystemMessage::CLASSIC_REGISTER_RECIEVED);
	sys->addRegister(rg, SystemMessage::CLASSIC_REGISTER_RECIEVED);
	return grpc::Status::OK;
}
grpc::Status ChannelService::SendEntangledRegister(grpc::ServerContext* context, 
	const QuantumMessage::EntangledRegisterMessage* request, 
	QuantumMessage::VoidMessage* reply) {
	System* sys = System::getInstance();
	shared_ptr<EntangledRegister> rg (&EntangledRegister::unserialize(request));
	sys->eve->doEvil(rg, SystemMessage::ENTANGLED_REGISTER_RECIEVED);
	sys->addRegister(rg, SystemMessage::ENTANGLED_REGISTER_RECIEVED);
	return grpc::Status::OK;
}
}
