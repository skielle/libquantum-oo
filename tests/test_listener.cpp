#include "channelListener.h"
#include "qubit.h"

int main() {

	shared_ptr<Qubit> Q = Qubit::create();
	shared_ptr<Qubit> R = Qubit::create();
	shared_ptr<Qubit> S = Qubit::create();

	QuantumChannel::ChannelListener server;

	server.setPort(50000);

	server.Run();
}
