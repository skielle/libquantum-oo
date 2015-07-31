/*
 * silentEvil.h
 * SilentEvil is an eavesdropper who simply listens to incoming traffic,
 * randomly measures qubits at a rate defined by evilness.  This represents
 * the attacker with no presence or purpose other than to analyze efficiency
 * of quantum algorithms in detecting eavesdropping.
 * SilentEvil implements the iEvil interface.
 */
#ifndef __QUANTUM_SILENTEVIL_H
#define __QUANTUM_SILENTEVIL_H

#include "iEvil.h"

namespace Quantum {
class SilentEvil : public iEvil {
	private:
		//the rate of measurement for quantum channels
		float evilness = 0.1;
	public:
		//doEvil - listen in on a message
		virtual void doEvil(shared_ptr<iRegister> reg, 
			SystemMessage message);
		//set the rate of random measurement for quantum channels
		void setEvilness(float newEvilness);
		//get the rate of random measurement for quantum channels
		float getEvilness();
};
}
#endif
