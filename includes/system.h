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
#include "iRunnable.h"

using namespace std;

namespace Quantum {

class System {
	private:
		shared_ptr<iRunnable> algorithm;
		System();
		static System* systemInstance;
		shared_ptr<QuantumChannel::ChannelListener> server;
	public:
		static System* getInstance();

		void RunSystem();
		int getListenerPort();
		void setListenerPort(int port);
		void runServer();
		void stopServer();
		void setAlgorithm(shared_ptr<iRunnable> a);
		void runAlgorithm();
};
}
#endif
