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
	int localIndex, int position, Matrix state, int result) {
	QuantumMessage::MeasurementMessage measurement;
	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	measurement.set_vectorindex(localIndex);
	measurement.set_position(position);
	measurement.mutable_m()->CopyFrom(state.serialize());
	measurement.set_result(result);

	grpc::Status status = 
		stub_->SendMeasurementMessage(
				&ctx, measurement, &rc);

	return status.ok();
}

bool ChannelService_client::SendQubit(shared_ptr<Qubit> q) {
	QuantumMessage::QubitMessage qm;
	QuantumMessage::RemoteIndexMessage rc;
	grpc::ClientContext ctx;

	qm = q->serialize();
	qm.set_pid(getpid());
	qm.set_callbackport(Channel::getServicePort());
	
	grpc::Status status = stub_->SendQubit(&ctx, qm, &rc);

	q->v->remoteQubits.at(q->position).remoteSystem =
		Channel::getIPFromCtxString(ctx.peer().data());
	q->v->remoteQubits.at(q->position).remotePort =
		Channel::getPortFromCtxString(ctx.peer().data());
	q->v->remoteQubits.at(q->position).remotePID = rc.remotepid();
	q->v->remoteQubits.at(q->position).remoteIndex = rc.remoteindex();

	/*
	printf("Sent qubit position %i, vector %i to system %s, vector %i.\r\n",
		q->position, q->v->getIndex(),
		q->v->remoteQubits.at(q->position).remoteSystem.c_str(),
		q->v->remoteQubits.at(q->position).remoteIndex);
	*/

	q->dereference();
	
	return status.ok();
}

bool ChannelService_client::SendClassicData(string theData) {
	QuantumMessage::ClassicMessage data;
	data.set_thedata(theData);
	data.set_callbackport(Channel::getServicePort());
	QuantumMessage::VoidMessage rc;
	grpc::ClientContext ctx;

	grpc::Status status = stub_->SendClassicData(&ctx, data, &rc);

	return status.ok();
}
}
