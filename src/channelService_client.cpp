/*
 * channelService_client.cpp
 */

#include <iostream>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#include <grpc/grpc.h>
#include <grpc++/support/channel_arguments.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <grpc++/support/status.h>
#include "quantumMessage.grpc.pb.h"
#include "quantumMessage.pb.h"
#include "qubit.h"
#include "channelService_client.h"
#include "channel.h"
#include "remotePeer.h"

using namespace std;
using namespace Quantum;
namespace QuantumChannel {
ChannelService_client::ChannelService_client(string server, int port)
	: stub_(QuantumMessage::QuantumChannel::NewStub(
		grpc::CreateChannel(
			server+":"+to_string(port),
			grpc::InsecureCredentials() //,
	//		grpc::ChannelArguments()
		)
	) ) {}

bool ChannelService_client::SendCallbackPort() {
	QuantumMessage::PortMessage portMsg;
	portMsg.set_port(Channel::getServicePort());
	portMsg.set_pid(getpid());
	QuantumMessage::PIDMessage pidMsg;
	grpc::ClientContext ctx;

	grpc::Status status = stub_->SendCallbackPort(&ctx, portMsg, &pidMsg);

	int pid = pidMsg.pid();

	RemotePeerList* rpl = RemotePeerList::getInstance();

	string peerIP = Channel::getIPFromCtxString(ctx.peer().data());
	int peerPort = Channel::getPortFromCtxString(ctx.peer().data());	

	int peerID = rpl->lookupPeerByServicePort(peerIP, peerPort);

	rpl->peerList.at(peerID).peerPID = pid;

	return status.ok();
}

bool ChannelService_client::SendQubit(shared_ptr<Qubit> q) {
	QuantumMessage::QubitMessage qm;
	qm = q->serialize();
	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	grpc::Status status = stub_->SendQubit(&ctx, qm, &rc);

	string remoteSystem = ctx.peer().data();


	return status.ok();
}

}
