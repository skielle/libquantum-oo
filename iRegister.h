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
		virtual	void setID(int id) {
			this->id = id;
		}
		virtual int getID() { return this->id; }
};
}
#endif
