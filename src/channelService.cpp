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
#include "system.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {

grpc::Status ChannelService::SendMeasurementMessage(
	grpc::ServerContext* context,
	const QuantumMessage::MeasurementMessage* request,
	QuantumMessage::VoidMessage* reply) {
	int i, j;
	bool synced = false;

	string peerIP = Channel::getIPFromCtxString(context->peer().data());
	
	QubitMap* qmap = QubitMap::getInstance();

	for ( i = 0; i < qmap->numStateVectors() && !synced; i++ ) {
		shared_ptr<StateVector> s = qmap->getStateVector(i);

		for ( j = 0; j < s->getWidth() && !synced; j++ ) {
			RemoteQubit rq = s->remoteQubits.at(j);
			if ( rq.remoteSystem != ""
				&& rq.remoteIndex == request->vectorindex() ) {
				s->qsv = Matrix::unserialize(&(request->m()));
				s->replay();
				s->measure(request->position(), 
					request->result(), false);

				synced = true;
			}
		}
	}

	return grpc::Status::OK;
}

grpc::Status ChannelService::SendClassicData(grpc::ServerContext* context,
	const QuantumMessage::ClassicMessage* request,
	QuantumMessage::VoidMessage* reply) {

	System* sys = System::getInstance();
	sys->addMessage(request->thedata());

	return grpc::Status::OK;
}	

grpc::Status ChannelService::SendQubit(grpc::ServerContext* context, 
	
	const QuantumMessage::QubitMessage* request, 
	QuantumMessage::RemoteIndexMessage* reply) {
	int i, j;
	int localIndex = 0;
	QubitMap* qm = QubitMap::getInstance();
	System* sys = System::getInstance();

	string remoteSystem = 
		Channel::getIPFromCtxString(context->peer().data());
	int remotePort = request->callbackport();
	int remotePID = request->pid();
	int remoteIndex = request->vectorindex();
	int position = request->position();

//unserialize the qubit
	shared_ptr<Qubit> q (&Qubit::unserialize(request));


//localize indexes for the qubit
	for ( i = 0; i < qm->numStateVectors() && localIndex == 0; i++ ) {
		shared_ptr<StateVector> localVector = qm->getStateVector(i);
		for (j = 0; j < localVector->remoteQubits.size() 
			&& localIndex == 0; j++ ) {
			if ( localVector->remoteQubits.at(j).remoteSystem 
					== remoteSystem
				&& localVector->remoteQubits.at(j).remotePID 
					== remotePID
				&& localVector->remoteQubits.at(j).remoteIndex 
					== remoteIndex ) {
//				localIndex = localVector->getIndex();
//				q->v = localVector;
//				q->v->remoteQubits.at(position).remoteSystem 
//					= "";
//TODO:  DO CATCHUP HERE
			}
		}
	}

	if ( localIndex == 0 ) {
		localIndex = QubitMap::getInstance()->getNewIndex();
		q->v->setIndex(localIndex);
		qm->addStateVector(q->v);
		for ( j = 0; j < q->v->remoteQubits.size(); j++ ) {
			if ( j != position ) {
				q->v->remoteQubits.at(j).remoteSystem 
					= remoteSystem;
				q->v->remoteQubits.at(j).remotePort 
					= remotePort;
				q->v->remoteQubits.at(j).remotePID 
					= remotePID;
				q->v->remoteQubits.at(j).remoteIndex 
					= remoteIndex;
			}
		}
			
	}

	q->origin.peerIP = remoteSystem;
	q->origin.peerServicePort = remotePort;
	
	qm->addQubit(q);
	sys->doEvil(q);

	reply->set_remoteindex(localIndex);
	reply->set_remotepid(getpid());

	sys->addMessage(qm->findQubit(localIndex, position));

	/*
	printf("Recv qubit position %i, vector %i from system %s, pid %i, vector %i\r\n",
		position, localIndex, remoteSystem.c_str(), 
		remotePID, remoteIndex);
	*/

	return grpc::Status::OK;
}

}
