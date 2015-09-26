/*
 * modelListenerService.h
 */

#ifndef __QUANTUMGUI_MODEL_LISTENER_SERVICE_H
#define __QUANTUMGUI_MODEL_LISTENER_SERVICE_H

#include <memory>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/status.h>
#include "qui.grpc.pb.h"

using namespace std;

namespace QuantumGUI {
class ModelListenerService final : public QuantumGUI::ModelView::Service {
	public:
	grpc::Status Polarize(grpc::ServerContext* context, 
		const QuantumGUI::PolarizationMessage* request, 
		QuantumMessage::VoidMessage* reply) override;
};
}

#endif
