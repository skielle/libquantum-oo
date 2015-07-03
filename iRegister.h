/*
 * iRegister.h
 */

#ifndef __QUANTUM_IREGISTER_H
#define __QUANTUM_IREGISTER_H

namespace Quantum {
class iRegister {
	protected:
		int id;
	public:
		void setID(int id) {
			this->id = id;
		}
		int getID() { return this->id; }
};
}
#endif
