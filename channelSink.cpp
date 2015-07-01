#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/server_credentials.h>
#include <grpc++/status.h>
#include "matrix.h"
#include "register.h"
#include "quantumMessage.grpc.pb.h"

using namespace std;
using namespace Quantum;

class QuantumMessageServiceImpl final : public QuantumMessage::QuantumChannel::Service {
	grpc::Status SendMatrix(grpc::ServerContext* context, const QuantumMessage::MatrixMessage* request, QuantumMessage::VoidMessage* reply) override {
		Matrix mx = Matrix::unserialize(request);
		mx.print();
		return grpc::Status::OK;
	}

	grpc::Status SendRegister(grpc::ServerContext* context, const QuantumMessage::RegisterMessage* request, QuantumMessage::VoidMessage* reply) override {
		Register rg = Register::unserialize(request);
		rg.print();
		return grpc::Status::OK;
	}
};

void RunSink() {
	string server_address("0.0.0.0:50051");
	QuantumMessageServiceImpl service;
	grpc::ServerBuilder builder;

	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	unique_ptr<grpc::Server> server(builder.BuildAndStart());
	cout<<"Server running on "<<server_address<<endl;
	server->Wait();
}

int main(int argc, char** argv) {
	RunSink();
	return 0;
}

