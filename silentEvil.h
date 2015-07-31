/*
 * silentEvil.h
 */
#ifndef __QUANTUM_SILENTEVIL_H
#define __QUANTUM_SILENTEVIL_H

#include "iEvil.h"

namespace Quantum {
class SilentEvil : public iEvil {
	private:
		float evilness = 0.1;
	public:
		virtual void doEvil(shared_ptr<iRegister> reg, SystemMessage message);
		void setEvilness(float newEvilness);
		float getEvilness();
};
}
#endif
