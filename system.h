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

#include "channelListener.h"
#include "iRegister.h"
#include "iRunnable.h"
#include "iEvil.h"
#include "systemMessage.h"

using namespace std;

namespace Quantum {

class System {
	private:
		shared_ptr<iRunnable> algorithm;
		vector<shared_ptr<iRegister>> registers;
		System();
		static System* systemInstance;
		queue<pair<SystemMessage, int>> messageQueue;
		shared_ptr<QuantumChannel::ChannelListener> server;
	public:
		static long mem;
		static long memmax;

		static unsigned long memman(long change);
		static System* getInstance();

		shared_ptr<iEvil> eve;
		
		bool isMessageQueueEmpty();
		SystemMessage getMessageType();
		int getMessageAddress();
		template<class messageType> 
		shared_ptr<messageType> getMessage() {
			shared_ptr<messageType> rx = 
				dynamic_pointer_cast<messageType> (
				this->registers.at( 
				this->messageQueue.front().second) );
			this->messageQueue.pop();
			return rx;
		}

		void RunSystem();
		int getListenerPort();
		void setListenerPort(int port);
		void runServer();
		void stopServer();
		void setAlgorithm(shared_ptr<iRunnable> a);
		void runAlgorithm();
		int addRegister(shared_ptr<iRegister> reg, 
			SystemMessage message);

		template<class registerType>
		shared_ptr<registerType> getRegister(int i) {
			shared_ptr<registerType> rg =
				dynamic_pointer_cast<registerType> (
				this->registers.at(i) );
			return rg;
		}
};
}
#endif
