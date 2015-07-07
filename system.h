/*
 * system.h
 */

#ifndef __QUANTUM_SYSTEM_H
#define __QUANTUM_SYSTEM_H

#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "iRegister.h"
#include "iRunnable.h"

using namespace std;

namespace Quantum {

enum class SystemMessage {
	REGISTER_RECIEVED = 1,
	CLASSIC_REGISTER_RECIEVED = 2
};

class System {
	private:
		shared_ptr<iRunnable> algorithm;
		vector<shared_ptr<iRegister>> registers;
		System();
		static System* systemInstance;
		queue<pair<SystemMessage, int>> messageQueue;
		int listenerPort = 50051;

	public:
		static long mem;
		static long memmax;

		static unsigned long memman(long change);
		static System* getInstance();

		bool isMessageQueueEmpty();
		SystemMessage getMessageType();
		template<class messageType> void getMessage() {
			shared_ptr<messageType> rx = 
				dynamic_pointer_cast<messageType> (
				this->registers.at( 
				this->messageQueue.front().second) );
			rx->print();
			this->messageQueue.pop();
//			return rx;
		}

		int getListenerPort();
		void setListenerPort(int port);
		void runServer();
		void runAlgorithm();
		int addRegister(shared_ptr<iRegister> reg, 
			SystemMessage message);
		shared_ptr<iRegister> getRegister(int i);
};
}
#endif
