/*
 * channelService.cpp
 */

#include <memory>
#include <thread>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/support/status.h>

#include "channel.h"
#include "channelService.h"
#include "channelService_client.h"
#include "remoteVectorMap.h"
#include "qubit.h"
#include "remotePeer.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {

void ChannelService::setServicePort(int port) {
	this->servicePort = port;
}

grpc::Status ChannelService::SendCallbackPort(grpc::ServerContext* context,
	const QuantumMessage::PortMessage* request,
	QuantumMessage::PIDMessage* reply) {
	
	string delim = ":";
	string peerIP;
	int peerClientPort;

	RemotePeerList* rpl = RemotePeerList::getInstance();

	int peerServicePort = request->port();

	peerIP = Channel::getIPFromCtxString(context->peer().data());
	peerClientPort = Channel::getPortFromCtxString(context->peer().data());

	int peerIndex = rpl->lookupPeerByServicePort(peerIP, peerServicePort);
	rpl->peerList.at(peerIndex).peerClientPort = peerClientPort;

	if ( rpl->peerList.at(peerIndex).peerPID == 0 ) {
		printf("Don't know peer pid.\r\n");
		QuantumChannel::ChannelService_client csc(
			rpl->peerList.at(peerIndex).peerIP,
			rpl->peerList.at(peerIndex).peerServicePort);
//TODO: what if this fails		csc.SendCallbackPort(this->servicePort);
		rpl->peerList.at(peerIndex).peerPID = -1;
		
	}

	reply->set_pid(getpid());
	return grpc::Status::OK;
}

grpc::Status ChannelService::SendQubit(grpc::ServerContext* context, 
	const QuantumMessage::QubitMessage* request, 
	QuantumMessage::VoidMessage* reply) {
	int localIndex = 0;
	string remoteSystem = context->peer().data();

//unserialize the qubit
	shared_ptr<Qubit> q (&Qubit::unserialize(request));

//localize indexes for the qubit
	localIndex = RemoteVectorMap::getInstance()->getLocalIndex(
		remoteSystem, q->v->getIndex());
	q->v->setIndex(localIndex);

//apply any catch-up operations on the qubit

//store the qubit in the system register?

	return grpc::Status::OK;
}

}
