/*
 * modelListenerService.cpp
 */

#include <memory>
#include <thread>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/status.h>
#include "channelService.h"
#include "channelService_client.h"
#include "gui.grpc.pb.h"

using namespace std;
using namespace Quantum;

namespace QuantumGUI {
grpc::Status ModelListenerService::Polarize(grpc::ServerContext* context, 
	const QuantumMessage::PolarizationMessage* request, 
	QuantumMessage::VoidMessage* reply) {

	printf("GOT ME A POLARIZATION REQUEST!");

	return grpc::Status::OK;
}
}
