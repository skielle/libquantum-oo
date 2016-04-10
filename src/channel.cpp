/*
 * channel.cpp
 */
//STD C++ includes
#include <string>

//QooSim includes
#include "channel.h"

using namespace std;

namespace QuantumChannel {

//default channel service port
int Channel::servicePort = 0;

/*
 * getIPFromCtxString
 * @param ctxData Google RPC context data
 * @return string containing the IP
 * Get the ip address from the context string
 */
string Channel::getIPFromCtxString(string ctxData) {
	string delim = ":";
	int firstDelim = ctxData.find(delim)+1;
	int secondDelim = ctxData.find(delim, firstDelim)+1;
	return ctxData.substr(firstDelim, secondDelim - firstDelim - 1 );
}

/*
 * getPortFromCtxString
 * @param ctxData Google RPC context data
 * @return the port
 * Get the port from the context string
 */
int Channel::getPortFromCtxString(string ctxData) {
	string delim = ":";
	int firstDelim = ctxData.find(delim)+1;
	int secondDelim = ctxData.find(delim, firstDelim)+1;
	return stoi(ctxData.substr(secondDelim, string::npos));
}

/*
 * setServicePort
 * @param port the port
 * Set the service port
 */
void Channel::setServicePort(int port) {
	Channel::servicePort = port;
}

/*
 * getServicePort
 * @return the port
 */
int Channel::getServicePort() {
	return Channel::servicePort;
}

}
