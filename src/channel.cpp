#include <string>

#include "channel.h"

using namespace std;

namespace QuantumChannel {

string Channel::getIPFromCtxString(string ctxData) {
	string delim = ":";
	int firstDelim = ctxData.find(delim)+1;
	int secondDelim = ctxData.find(delim, firstDelim)+1;
	return ctxData.substr(firstDelim, secondDelim - firstDelim - 1 );
}

int Channel::getPortFromCtxString(string ctxData) {
	string delim = ":";
	int firstDelim = ctxData.find(delim)+1;
	int secondDelim = ctxData.find(delim, firstDelim)+1;
	return stoi(ctxData.substr(secondDelim, string::npos));
}

}
