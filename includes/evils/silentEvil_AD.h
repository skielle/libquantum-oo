/*
 * silentEvil.h
 * SilentEvil is an eavesdropper who simply listens to incoming traffic,
 * randomly measures qubits at a rate defined by evilness.  This represents
 * the attacker with no presence or purpose other than to analyze efficiency
 * of quantum algorithms in detecting eavesdropping.
 * SilentEvil implements the iEvil interface.
 * Additionally, this implementation adds amplitude damping for compound evil
 */
#ifndef __QUANTUM_SILENTEVIL_AD_H
#define __QUANTUM_SILENTEVIL_AD_H

#include "qubit.h"
#include "iEvil.h"

namespace Quantum {
class SilentEvil_AD : public iEvil {
	private:
		//the rate of measurement for quantum channels
		float evilness = .05;
		//the rate of measurement for quantum channels
		float eta = .05;
	public:
		//doEvil - listen in on a message
		virtual void doEvil(shared_ptr<Qubit> q);
		//set the rate of random measurement for quantum channels
		void setEvilness(float newEvilness);
		//get the rate of random measurement for quantum channels
		float getEvilness();
		//set the rate of random measurement for quantum channels
		void setEta(float newEta);
		//get the rate of random measurement for quantum channels
		float getEta();
};
}
#endif
