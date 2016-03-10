#include <queue>
#include <string>
#include <vector>

#include "channelService_client.h"

namespace Quantum {
class ClientConnectionContext {
	private:
		queue<pair<SystemMessage, iRegister>> messageQueue;
	public:
		string identifier; 
		ChannelService_client csc;
	
		ClientConnectionContext(string identifier, 
			ChannelService_client csc);
	
		SystemMessage getMessageType();
		shared_ptr<messageType> getMessage() {
			shared_ptr<messageType> rx =
				dynamic_pointer_cast<messageType> (
					this->messageQueue.front().second) );
			this->messageQueue.pop();
			return rx;
		}

		void insertMessage(SystemMessage msg, 
			shared_ptr<iRegister> reg);
}
