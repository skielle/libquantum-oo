/*
 * rotationError.h
 */
#ifndef __QUANTUM_ROTATIONERROR_H
#define __QUANTUM_ROTATIONERROR_H

#include "qubit.h"
#include "iEvil.h"

namespace Quantum {
class RotationError : public iEvil {
	private:
		//the rate of measurement for quantum channels
		float theta = .05;
	public:
		//doEvil - listen in on a message
		virtual void doEvil(shared_ptr<Qubit> q);
		//set the rate of random measurement for quantum channels
		void setTheta(float newTheta);
		//get the rate of random measurement for quantum channels
		float getTheta();
};
}
#endif
