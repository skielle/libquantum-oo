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
#include "qubit.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
grpc::Status ChannelService::SendQubit(grpc::ServerContext* context, 
	const QuantumMessage::QubitMessage* request, 
	QuantumMessage::VoidMessage* reply) {

	printf("QUBIT RECIEVED\r\n");
	printf("PEER: %s\n", context->peer().data());

	shared_ptr<Qubit> q (&Qubit::unserialize(request));

	printf("Qubit position: %i\r\n", q->position);

	return grpc::Status::OK;
}

}
