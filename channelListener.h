/*
 * channelListener.h
 */

#ifndef __QUANTUMCHANNEL_CHANNEL_LISTENER_H
#define __QUANTUMCHANNEL_CHANNEL_LISTENER_H

#include <grpc++/server.h>
#include <memory>
#include "iRunnable.h"

using namespace Quantum;
using namespace std;

namespace QuantumChannel {
class ChannelListener : public iRunnable {
	private:
		int port;
		unique_ptr<grpc::Server> server;
	public:
		int getPort();
		void setPort(int inPort);
		void Run();
		void Stop();
		
};
}

#endif
