#include "channelListener.h"

int main() {
	QuantumChannel::ChannelListener server;

	server.setPort(50000);

	server.Run();
}
