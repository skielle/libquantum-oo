/*
 * channelService_client.cpp
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/channel_arguments.h>
#include <grpc++/channel_interface.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/credentials.h>
#include <grpc++/status.h>
#include "quantumMessage.grpc.pb.h"
#include "entangledRegister.h"
#include "register.h"
#include "channelService_client.h"
#include "entanglement.h"

using namespace std;
using namespace Quantum;
namespace QuantumChannel {
ChannelService_client::ChannelService_client(string server, int port)
	: stub_(QuantumMessage::QuantumChannel::NewStub(
		grpc::CreateChannel(
			server+":"+to_string(port),
			grpc::InsecureCredentials(),
			grpc::ChannelArguments()
		)
	) ) {}

bool ChannelService_client::SendRegister(Register r) {
	QuantumMessage::RegisterMessage rm;
	rm = r.serialize();
	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	grpc::Status status = stub_->SendRegister(&ctx, rm, &rc);

	return status.ok();
}

bool ChannelService_client::SendClassicRegister(ClassicRegister r) {
	QuantumMessage::ClassicRegisterMessage rm;
	rm = r.serialize();
	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	grpc::Status status = stub_->SendClassicRegister(&ctx, rm, &rc);

}

bool ChannelService_client::SendEntangledRegister(EntangledRegister r) {
	QuantumMessage::EntangledRegisterMessage rm;
	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	shared_ptr<ChannelService_client> csc (this);

	rm = r.serialize();
	if ( r.isAleph() ) {
		r.getEntanglement()->makeAlephRemote(shared_from_this());
	} else {
		r.getEntanglement()->makeBeitRemote(shared_from_this());
	}

	grpc::Status status = stub_->SendEntangledRegister(&ctx, rm, &rc);
}

bool ChannelService_client::EventPairMeasureFinish(
	QuantumMessage::EntangledMeasurementMessage em) {

	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	grpc::Status status = 
		stub_->EventPairMeasureFinish(&ctx, em, &rc);
}
}
