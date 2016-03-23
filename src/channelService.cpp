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
#include "__stub_remoteQubit.h"
#include "qubit.h"
#include "qubitMap.h"
#include "remotePeer.h"
#include "channel.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {

grpc::Status ChannelService::SendMeasurementMessage(
	grpc::ServerContext* context,
	const QuantumMessage::MeasurementMessage* request,
	QuantumMessage::VoidMessage* reply) {
	int i;

	string peerIP = Channel::getIPFromCtxString(context->peer().data());
	
	QubitMap* qmap = QubitMap::getInstance();

	for ( i = 0; i < qmap->numQubits(); i++ ) {
		RemoteQubit rq = qmap->getQubit(i)->v->remoteQubits.at(
			qmap->getQubit(i)->position);
		if ( rq.remoteSystem != "" 
			&& rq.remoteIndex == request->vectorindex() ) {
			qmap->getQubit(i)->v->measure(request->position(), 
				request->result(), false);
			break;
		}
	}

	return grpc::Status::OK;
}

grpc::Status ChannelService::SendCallbackPort(grpc::ServerContext* context,
	const QuantumMessage::PortMessage* request,
	QuantumMessage::PIDMessage* reply) {
	
	RemotePeerList* rpl = RemotePeerList::getInstance();

	int peerServicePort = request->port();
	string peerIP = Channel::getIPFromCtxString(context->peer().data());

	int peerIndex = rpl->lookupPeerByServicePort(peerIP, peerServicePort);

	rpl->peerList.at(peerIndex).peerPID = request->pid();

	reply->set_pid(getpid());
	return grpc::Status::OK;
}

grpc::Status ChannelService::SendQubit(grpc::ServerContext* context, 
	const QuantumMessage::QubitMessage* request, 
	QuantumMessage::RemoteIndexMessage* reply) {
	int i, j;
	int localIndex = 0;
	QubitMap* qm = QubitMap::getInstance();

	string remoteSystem = 
		Channel::getIPFromCtxString(context->peer().data());
	int remotePort = Channel::getPortFromCtxString(context->peer().data());
	int remotePID = request->pid();
	int remoteIndex = request->vectorindex();
	int position = request->position();

//unserialize the qubit
	shared_ptr<Qubit> q (&Qubit::unserialize(request));

//localize indexes for the qubit
	for ( i = 0; i < qm->numStateVectors(); i++ ) {
		shared_ptr<StateVector> localVector = qm->getStateVector(i);
		for (j = 0; j < localVector->remoteQubits.size(); j++ ) {
			if ( localVector->remoteQubits.at(j).remoteSystem 
					== remoteSystem
				&& localVector->remoteQubits.at(j).remotePID 
					== remotePID
				&& localVector->remoteQubits.at(j).remoteIndex 
					== remoteIndex ) {
				localIndex = localVector->getIndex();
				q->v = localVector;
//TODO:  DO CATCHUP HERE
			}
		}
	}

	if ( localIndex == 0 ) {
		localIndex = QubitMap::getInstance()->getNewIndex();
		q->v->setIndex(localIndex);
		qm->addStateVector(q->v);
	}

	q->v->remoteQubits.at(position).remoteSystem = remoteSystem;
	q->v->remoteQubits.at(position).remotePort = remotePort;
	q->v->remoteQubits.at(position).remotePID = remotePID;
	q->v->remoteQubits.at(position).remoteIndex = remoteIndex;
	qm->addQubit(q);

	reply->set_remoteindex(localIndex);
	reply->set_remotepid(getpid());
	printf("Recv qubit position %i, vector %i from system %s, pid %i, vector %i\r\n",
		position, localIndex, remoteSystem.c_str(), 
		remotePID, remoteIndex);

	return grpc::Status::OK;
}

}
