/*
 * channelSource.cpp
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/channel_arguments.h>
#include <grpc++/channel_interface.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/credentials.h>
#include <grpc++/status.h>
#include "quantumMessage.grpc.pb.h"
#include "matrix.h"
#include "register.h"
#include "gates.cpp"

using namespace std;
using namespace Quantum;
class ChannelSource {
	public:
		ChannelSource(shared_ptr<grpc::ChannelInterface> channel)
			: stub_(QuantumMessage::QuantumChannel::NewStub(channel)) {}

		bool SendRegister(Register m) {
			QuantumMessage::MatrixMessage mm;
			mm = m.serialize();
			QuantumMessage::ResponseCode rc;
			grpc::ClientContext ctx;

			grpc::Status status = stub_->SendRegister(&ctx,
				mm, &rc);
	
			if ( status.ok() ) {
			
			}
			
			return rc.responsecode();
		}
	
		bool SendMatrix(Matrix m) {
			QuantumMessage::MatrixMessage mm;
			mm = m.serialize();
			QuantumMessage::ResponseCode rc;
			grpc::ClientContext ctx;

			grpc::Status status = stub_->SendMatrix(&ctx,
				mm, &rc);
	
			if ( status.ok() ) {
			
			}
			return rc.responsecode();
		}
	
	private:
		unique_ptr<QuantumMessage::QuantumChannel::Stub> stub_;
};

int main ( int argc, char** argv ) {
	ChannelSource cs(grpc::CreateChannel("localhost:50051",
			grpc::InsecureCredentials(),
			grpc::ChannelArguments()));

	Register rg = Register((MAX_UNSIGNED)0, 4);

	rg.applyGate(new Hadamard(), 0);	
	rg.applyGate(new Hadamard(), 1);	
	rg.applyGate(new Hadamard(), 2);	
	rg.applyGate(new Hadamard(), 3);	

	cout<<"Sending..."<<endl;
	rg.print();

	if(cs.SendRegister(rg)) {
		printf("Send succeeded!\n");
	} else {
		printf("Send failed!\n");
	}

	return 0;
}


