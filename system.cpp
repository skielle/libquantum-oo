#include "system.h"

namespace Quantum {
long System::mem = 0;
long System::memmax = 0;

unsigned long System::memman(long change) {
	System::mem += change;
	if ( System::mem > System::memmax ) {
		System::memmax = System::mem;
	}
	return System::mem;
}
}
