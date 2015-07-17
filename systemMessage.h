/*
 * systemMessage.h
 */

#ifndef __QUANTUM_SYSTEM_MESSAGE_H
#define __QUANTUM_SYSTEM_MESSAGE_H

namespace Quantum {
class SystemMessage {
	private:
		int _messageType;
	public:
		static const int DEFAULT_MESSAGE = 0;
		static const int CLASSIC_REGISTER_RECIEVED = 1;
		static const int REGISTER_RECIEVED = 2;
 
		SystemMessage();
		SystemMessage(int MessageType);
		int getMessageType();
		void setMessageType(int MessageType);
		bool operator ==(SystemMessage &sm);
		bool operator ==(int smt);
};
}
#endif
