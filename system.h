#include <memory>
#include <utility>
#include <queue>
#include <vector>
#include <string>
#include "register.h"

using namespace std;

#ifndef __QUANTUM_SYSTEM_H
#define __QUANTUM_SYSTEM_H
namespace Quantum {

enum class SystemMessage {
	REGISTER_RECIEVED = 1
};

class System {
	private:
		vector<Register> registers;
shared_ptr<Register>  rxfake;
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
		int addRegister(Register reg);
		Register getRegister(int i);
};
}
#endif
