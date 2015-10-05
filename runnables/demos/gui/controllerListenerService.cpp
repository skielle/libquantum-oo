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

	shared_ptr<const PolarizationMessage> rmsg =
		make_shared<const PolarizationMessage>(*request);
	shared_ptr<const google::protobuf::Message> grmsg = 
		dynamic_pointer_cast<const PolarizationMessage>(rmsg);

	this->requestQueue.push(pair<string, 
		shared_ptr<const google::protobuf::Message>>
		("Polarize", grmsg));
	this->controller->Process();
	if ( !this->responseQueue.empty() ) {
//		reply = (QuantumGUI::VoidMessage*)this->responseQueue.front();
		this->responseQueue.pop();
	}
	
	fflush(stdout);
	return grpc::Status::OK;
}

grpc::Status ControllerListenerService::Measure(grpc::ServerContext* context,
	const QuantumGUI::RegisterAddressMessage* request,
	QuantumGUI::ResultMessage* reply) {

	shared_ptr<const RegisterAddressMessage> rmsg =
		make_shared<const RegisterAddressMessage> (*request);
	shared_ptr<const google::protobuf::Message> grmsg = 
		dynamic_pointer_cast<const RegisterAddressMessage>(rmsg);

	this->requestQueue.push(pair<string, 
		shared_ptr<const google::protobuf::Message>>
		("Measurement", grmsg));
	this->controller->Process();
	if ( !this->responseQueue.empty() ) {
		shared_ptr<google::protobuf::Message> resm = 
			this->responseQueue.front();
		this->responseQueue.pop();
		
		reply->set_registeraddress(((dynamic_pointer_cast<ResultMessage> (resm)).get())->registeraddress());
//		reply = (dynamic_pointer_cast<ResultMessage> (resm)).get();
printf("ll: %i\r\n", reply->registeraddress());
	}
	
	fflush(stdout);
	return grpc::Status::OK;
}

grpc::Status ControllerListenerService::GetRegisterStatus(
	grpc::ServerContext* context,
	const QuantumGUI::VoidMessage* request,
	QuantumGUI::RegisterStatusMessage* reply) {

	shared_ptr<const VoidMessage> rmsg (request);
	shared_ptr<const google::protobuf::Message> grmsg = 
		dynamic_pointer_cast<const PolarizationMessage>(rmsg);

	this->requestQueue.push(pair<string, 
		shared_ptr<const google::protobuf::Message>>
		("Register Status", grmsg));
	//this->controller->Process();
	if ( !this->responseQueue.empty() ) {
//		reply = (QuantumGUI::RegisterStatusMessage*)this->responseQueue.front();
		this->responseQueue.pop();
	}
	
	fflush(stdout);
	return grpc::Status::OK;
}
}
