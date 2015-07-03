#include "system.h"

namespace Quantum {
long System::mem = 0;
long System::memmax = 0;

System* System::systemInstance = 0;

unsigned long System::memman(long change) {
	System::mem += change;
	if ( System::mem > System::memmax ) {
		System::memmax = System::mem;
	}
	return System::mem;
}

System::System () {
}

System* System::getInstance() {
	if ( System::systemInstance == 0 ) {
		System::systemInstance = new System();
	}
	return System::systemInstance;
}

int System::addRegister(iRegister* reg) {
	registers.push_back(reg);
	return registers.size() - 1;
}

iRegister* System::getRegister(int hash) {
	return registers.at(hash);
}

}
