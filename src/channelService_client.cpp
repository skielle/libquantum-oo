/*
 * channelService_client.cpp
 */
//STD C++ includes
#include <iostream>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unistd.h>

//Google RPC includes
#include <grpc/grpc.h>
#include <grpc++/support/channel_arguments.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <grpc++/support/status.h>

//QooSim includes
#include "channel.h"
#include "channelService_client.h"
#include "quantumMessage.grpc.pb.h"
#include "quantumMessage.pb.h"
#include "qubit.h"
#include "remotePeer.h"
#include "__stub_remoteQubit.h"

using namespace std;
using namespace Quantum;
namespace QuantumChannel {

/*
 * ChannelService_client
 * @param server
 * @param port
 * Create the Channel Service client
 */
ChannelService_client::ChannelService_client(string server, int port)
	: stub_(QuantumMessage::QuantumChannel::NewStub(
		grpc::CreateChannel(
			server+":"+to_string(port),
			grpc::InsecureCredentials() 
		)
	) ) {}
/*
 * SendMeasurementMessage
 * @param localIndex the local state vector index
 * @param position the position of the qubit in the vector
 * @param state the state vector
 * @param result the measurement result
 * Inform peers in the same quantum state that a measurement has been performed
 */
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

/*
 * SendQubit
 * @param q the qubit to send
 * Send a qubit to a remote system
 */
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

	q->dereference();
	
	return status.ok();
}

/*
 * SendClassicData
 * @param theData the data to send
 * Send classic bits to a remote peer
 */
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
