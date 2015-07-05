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

#include "register.h"

using namespace std;

namespace Quantum {

enum class SystemMessage {
	REGISTER_RECIEVED = 1
};

class System {
	private:
		vector<shared_ptr<iRegister>> registers;
		System();
		static System* systemInstance;
		queue<pair<SystemMessage, int>> messageQueue;

	public:
		static long mem;
		static long memmax;

		static unsigned long memman(long change);
		static System* getInstance();

		void runServer();
		void runAlgorithm();
		int addRegister(shared_ptr<iRegister> reg);
		shared_ptr<iRegister> getRegister(int i);
};
}
#endif
