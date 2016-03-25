/*
 * channelService_client.h
 */

#ifndef __QUANTUM_CHANNEL_SERVICE_CLIENT_H
#define __QUANTUM_CHANNEL_SERVICE_CLIENT_H

#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/create_channel.h>
#include "quantumMessage.grpc.pb.h"
#include "quantumMessage.pb.h"
#include "qubit.h"

using namespace std;
using namespace Quantum;

namespace QuantumChannel {
class ChannelService_client :
	public std::enable_shared_from_this<ChannelService_client> {
	private:
		unique_ptr<QuantumMessage::QuantumChannel::Stub> stub_;
	public:
		ChannelService_client(string server, int port);

		bool SendMeasurementMessage(
			int localIndex, int position, int result);
		bool SendQubit(shared_ptr<Qubit> q);
		bool SendClassicData(string theData);
};
}
#endif
