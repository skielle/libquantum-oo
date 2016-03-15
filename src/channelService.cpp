/*
 * channelService.cpp
 */

#include <memory>
#include <thread>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/support/status.h>
#include "channelService.h"
#include "channelService_client.h"
#include "remoteVectorMap.h"
#include "qubit.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
grpc::Status ChannelService::SendQubit(grpc::ServerContext* context, 
	const QuantumMessage::QubitMessage* request, 
	QuantumMessage::VoidMessage* reply) {
	int localIndex = 0;
	string remoteSystem = context->peer().data();

	printf("QUBIT RECIEVED\r\n");
	printf("PEER: %s\n", remoteSystem.c_str());

//unserialize the qubit
	shared_ptr<Qubit> q (&Qubit::unserialize(request));

//localize indexes for the qubit
	printf("Remote system: %s\r\nRemote index: %i\r\n", 
		remoteSystem.c_str(), q->v->getIndex());
	localIndex = RemoteVectorMap::getInstance()->getLocalIndex(
		remoteSystem, q->v->getIndex());
	printf("Local index: %i\r\n", localIndex);
	q->v->setIndex(localIndex);

//apply any catch-up operations on the qubit

//store the qubit in the system register?
	q->print();

	return grpc::Status::OK;
}

}
