/*
 * concreteController.cpp
 */

#include <google/protobuf/message.h>
#include <queue>
#include <string>
#include <utility>

#include "controllerListener.h"
#include "concreteController.h"
#include "iController.h"
#include "register.h"
#include "gates.cpp"

namespace QuantumGUI {
shared_ptr<ConcreteController> ConcreteController::create() {
	return shared_ptr<ConcreteController>(new ConcreteController);
}

void ConcreteController::Run() {
	ControllerListener cl = ControllerListener();
	cl.setController(shared_from_this());
	cl.setPort(60000);
	cl.Run();
}

void ConcreteController::Process() {
	pair<string, shared_ptr<const google::protobuf::Message>> regm = 
		this->service->requestQueue.front();
	this->service->requestQueue.pop();

	if ( regm.first == "Polarize" ) {
		shared_ptr<const PolarizationMessage> pMessage =
			dynamic_pointer_cast<const PolarizationMessage>
			(regm.second);

		RotateX* rx = new RotateX();
		rx->setPsi(pMessage->angle()/180 * pi);

		this->modelRegister.applyGate(rx, pMessage->registeraddress());
		
		this->modelRegister.print();

		printf("Rotating %i by %f\r\n", pMessage->registeraddress(),
			pMessage->angle());
	} 

	if ( regm.first == "Measurement" ) {
		shared_ptr<const RegisterAddressMessage> mMessage =
			dynamic_pointer_cast<const RegisterAddressMessage>
			(regm.second);

		int measuredValue = 
			this->modelRegister.measure(
				mMessage->registeraddress());
		this->modelRegister.print();
	
		printf("Measured %i, got %i\r\n", 
			mMessage->registeraddress(), measuredValue);

		shared_ptr<ResultMessage> mVmsg = 
			make_shared<ResultMessage> ( ResultMessage() );
		mVmsg->set_registeraddress(measuredValue);
		this->service->responseQueue.push(mVmsg);
	}

	if ( regm.first == "Register Status" ) {
		printf("Got message type: %s\r\n", regm.first.c_str());
	}
}

ConcreteController::ConcreteController() :
	modelRegister(Register((long long unsigned int)0, 4)){
}
	
}
