#ifndef __QUANTUM_SYSTEM_H
#define __QUANTUM_SYSTEM_H

namespace Quantum {
class System {
	public:

		static long mem;
		static long memmax;

		static unsigned long memman(long change);
};
}
#endif
