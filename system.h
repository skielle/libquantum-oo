#include <vector>
#include <string>
#include "iRegister.h"

using namespace std;

#ifndef __QUANTUM_SYSTEM_H
#define __QUANTUM_SYSTEM_H
namespace Quantum {
class System {
	private:
		vector<iRegister *> registers;
		System();
		static System* systemInstance;

	public:
		static long mem;
		static long memmax;

		static unsigned long memman(long change);
		static System* getInstance();

		int addRegister(iRegister* reg);
		iRegister* getRegister(int i);
};
}
#endif
