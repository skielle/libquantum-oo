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
#include "channelService_client.h"
#include "classicRegister.h"
#include "entangledRegister.h"
#include "entanglement.h"
#include "register.h"
#include "stub__networkRegister.h"
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
	shared_ptr<EntangledRegister> rg = EntangledRegister::unserialize(request);
	shared_ptr<QuantumChannel::ChannelService_client>
		 csc( new QuantumChannel::ChannelService_client(
			"127.0.0.1", 50101) );

	if ( rg->isAleph() ) {
		rg->getEntanglement()->makeBeitRemote(csc);
	} else {
		rg->getEntanglement()->makeAlephRemote(csc);
	}
	sys->eve->doEvil(rg, SystemMessage::ENTANGLED_REGISTER_RECIEVED);
	sys->addRegister(rg, SystemMessage::ENTANGLED_REGISTER_RECIEVED);
	return grpc::Status::OK;
}

grpc::Status ChannelService::EventPairMeasureFinish(
	grpc::ServerContext* context,
	const QuantumMessage::EntangledMeasurementMessage* request,
	QuantumMessage::VoidMessage* reply) {
	System* sys = System::getInstance();
	
	printf("ENTANGLEMENT MEASUREMENT MESSAGE RECIEVED\n");

	return grpc::Status::OK;
}

}
