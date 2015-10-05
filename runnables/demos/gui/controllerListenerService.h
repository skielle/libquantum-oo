/*
 * controllerListenerService.h
 */

#ifndef __QUANTUMGUI_CONTROLLER_LISTENER_SERVICE_H
#define __QUANTUMGUI_CONTROLLER_LISTENER_SERVICE_H

#include <memory>
#include <queue>
#include <string.h>
#include <utility>

#include <google/protobuf/message.h>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/support/status.h>
#include "gui.grpc.pb.h"

using namespace std;

namespace QuantumGUI {
class iController;
class ControllerListenerService final : 
public QuantumGUI::ControllerView::Service {
	private:
		shared_ptr<iController> controller;
	public:
		queue<pair<string, 
			shared_ptr<const google::protobuf::Message>>>
			requestQueue;
		queue<shared_ptr<google::protobuf::Message>> responseQueue;

		void setController(shared_ptr<iController> inController); 

		grpc::Status Polarize(grpc::ServerContext* context, 
			const QuantumGUI::PolarizationMessage* request, 
			QuantumGUI::VoidMessage* reply) override;
		grpc::Status Measure(grpc::ServerContext* context, 
			const QuantumGUI::RegisterAddressMessage* request, 
			QuantumGUI::ResultMessage* reply) override;
		grpc::Status GetRegisterStatus(grpc::ServerContext* context, 
			const QuantumGUI::VoidMessage* request, 
			QuantumGUI::RegisterStatusMessage* reply) override;
};
}

#endif
