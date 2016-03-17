/*
 * channel.h
 */

#ifndef __QUANTUMCHANNEL_CHANNEL_H
#define __QUANTUMCHANNEL_CHANNEL_H

#include <string>

#include "channel.h"

using namespace std;

namespace QuantumChannel {
class Channel {
	private:
		static int servicePort; 
	public:
		static string getIPFromCtxString(string ctxData);
		static int getPortFromCtxString(string ctxData);
		static void setServicePort(int port);
		static int getServicePort();
};
}

#endif
