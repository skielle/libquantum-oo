/*
 * controllerListenerService.cpp
 */

#include <memory>
#include <queue>
#include <thread>
#include <utility>

#include <google/protobuf/message.h>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/support/status.h>
#include "controllerListenerService.h"
#include "gui.grpc.pb.h"

#include "iController.h"

using namespace std;
using namespace QuantumGUI;

namespace QuantumGUI {
void ControllerListenerService::setController(shared_ptr<iController> inController) {
	this->controller = inController;
}

grpc::Status ControllerListenerService::Polarize(grpc::ServerContext* context, 
	const QuantumGUI::PolarizationMessage* request, 
	QuantumGUI::VoidMessage* reply) {

	printf("GOT ME A POLARIZATION REQUEST!\r\n");
	this->requestQueue.push(pair<string, const google::protobuf::Message*>
		("Polarize", request));
	this->controller->Process();
	if ( !this->responseQueue.empty() ) {
		reply = (QuantumGUI::VoidMessage*)this->responseQueue.front();
		this->responseQueue.pop();
	}
	
	fflush(stdout);
	return grpc::Status::OK;
}

grpc::Status ControllerListenerService::Measure(grpc::ServerContext* context,
	const QuantumGUI::RegisterAddressMessage* request,
	QuantumGUI::ResultMessage* reply) {

	printf("GOT ME A MEASUREMENT REQUEST!\r\n");
	this->requestQueue.push(pair<string, const google::protobuf::Message*>
		("Measurement", request));
	//this->controller->Process();
	if ( !this->responseQueue.empty() ) {
		reply = (QuantumGUI::ResultMessage*)this->responseQueue.front();
		this->responseQueue.pop();
	}
	
	fflush(stdout);
	return grpc::Status::OK;
}

grpc::Status ControllerListenerService::GetRegisterStatus(
	grpc::ServerContext* context,
	const QuantumGUI::VoidMessage* request,
	QuantumGUI::RegisterStatusMessage* reply) {

	printf("GOT ME A REGISTER STATUS REQUEST!\r\n");
	this->requestQueue.push(pair<string, const google::protobuf::Message*>
		("Register Status", request));
	//this->controller->Process();
	if ( !this->responseQueue.empty() ) {
		reply = (QuantumGUI::RegisterStatusMessage*)this->responseQueue.front();
		this->responseQueue.pop();
	}
	
	fflush(stdout);
	return grpc::Status::OK;
}
}
