/*
 * systemMessage.h
 * This class defines the type of network message recieved by a channelService.
 * Since registers (messages) are placed on the message queue as iRegister
 * objects, the systemMessage value helps determine which type of register to
 * pop off the queue.  Note that since the message type is an int, additional
 * system messages may be defined by users of this library.
 */

#ifndef __QUANTUM_SYSTEM_MESSAGE_H
#define __QUANTUM_SYSTEM_MESSAGE_H

namespace Quantum {
class SystemMessage {
	private:
		//The int to store the message type
		int _messageType;
	public:
		//Some int values for "built in" message types
		static const int DEFAULT_MESSAGE = 0;
		static const int CLASSIC_DATA_RECEIVED = 1;
		static const int QUANTUM_DATA_RECEIVED = 2;
 
		//Default constructor
		SystemMessage();
		//Constructor, takes the message type to create as an argument
		SystemMessage(int MessageType);
		//get the message type of this system message
		int getMessageType();
		//set the message type of this system message
		void setMessageType(int MessageType);
		//equality operator overload for comparison with other system
		//messages
		bool operator ==(SystemMessage &sm);
		//equality operator overload for comparison with ints
		bool operator ==(int smt);
};
}
#endif
