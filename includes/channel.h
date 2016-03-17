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
	public:
		static string getIPFromCtxString(string ctxData);
		static int getPortFromCtxString(string ctxData);
};
}

#endif
