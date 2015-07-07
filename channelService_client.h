/*
 * channelService_client.h
 */

#ifndef __QUANTUM_CHANNEL_SERVICE_CLIENT_H
#define __QUANTUM_CHANNEL_SERVICE_CLIENT_H

#include <string>

#include <grpc/grpc.h>
#include <grpc++/channel_interface.h>
#include <grpc++/create_channel.h>
#include "quantumMessage.grpc.pb.h"
#include "classicRegister.h"
#include "register.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
class ChannelService_client {
	private:
		unique_ptr<QuantumMessage::QuantumChannel::Stub> stub_;
	public:
		ChannelService_client(string server, int port);

		bool SendRegister(Register r);
		bool SendClassicRegister(ClassicRegister r);
};
}
#endif
