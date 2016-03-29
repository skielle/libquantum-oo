/*
 * amplitudeDamping_dual.h
 */
#ifndef __QUANTUM_AMPLITUDEDAMPING_DUAL_H
#define __QUANTUM_AMPLITUDEDAMPING_DUAL_H

#include "qubit.h"
#include "iEvil.h"

namespace Quantum {
class AmplitudeDamping_dual : public iEvil {
	private:
		//the rate of measurement for quantum channels
		float eta = .05;
	public:
		//doEvil - listen in on a message
		virtual void doEvil(shared_ptr<Qubit> q);
		//set the rate of random measurement for quantum channels
		void setEta(float newEta);
		//get the rate of random measurement for quantum channels
		float getEta();
};
}
#endif
