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
#include "__stub_remoteQubit.h"

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

bool ChannelService_client::SendMeasurementMessage(
	int localIndex, int position, int result) {
	QuantumMessage::MeasurementMessage measurement;
	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	measurement.set_vectorindex(localIndex);
	measurement.set_position(position);
	measurement.set_pid(getpid());
	measurement.set_result(result);

	grpc::Status status = 
		stub_->SendMeasurementMessage(
				&ctx, measurement, &rc);

	return status.ok();
}

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
	QuantumMessage::RemoteIndexMessage rc;
	grpc::ClientContext ctx;

	qm = q->serialize();
	qm.set_pid(getpid());
	
	grpc::Status status = stub_->SendQubit(&ctx, qm, &rc);

	q->v->remoteQubits.at(q->position).remoteSystem =
		Channel::getIPFromCtxString(ctx.peer().data());
	q->v->remoteQubits.at(q->position).remotePort =
		Channel::getPortFromCtxString(ctx.peer().data());
	q->v->remoteQubits.at(q->position).remotePID = rc.remotepid();
	q->v->remoteQubits.at(q->position).remoteIndex = rc.remoteindex();

	printf("Sent qubit position %i, vector %i to system %s, vector %i.\r\n",
		q->position, q->v->getIndex(),
		q->v->remoteQubits.at(q->position).remoteSystem.c_str(),
		q->v->remoteQubits.at(q->position).remoteIndex);
	
	q->dereference();
	
	return status.ok();
}

}
