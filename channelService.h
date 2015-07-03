/*
 * channelService.h
 */

#ifndef __QUANTUMCHANNEL_CHANNEL_SERVICE_H
#define __QUANTUMCHANNEL_CHANNEL_SERVICE_H

#include <memory>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/status.h>
#include "quantumMessage.grpc.pb.h"

using namespace std;

namespace QuantumChannel {
class ChannelService final : public QuantumMessage::QuantumChannel::Service {
	grpc::Status SendRegister(grpc::ServerContext* context, 
		const QuantumMessage::RegisterMessage* request, 
		QuantumMessage::VoidMessage* reply) override;
};
}

#endif
