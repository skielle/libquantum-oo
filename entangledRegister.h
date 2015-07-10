/*
 * entangledRegister.h
 */

#include <vector>
#include "register.h"
#include "config.h"
#include "matrix.h"

#ifndef __QUANTUM_ENTANGLED_REGISTER_H
#define __QUANTUM_ENTANGLED_REGISTER_H

namespace Quantum {
class Entanglement;

class EntangledRegister: public Register {
	protected:
		Entanglement* ent;
		vector<Matrix>** opHistory;
		bool aleph;

		void revert(int target);
		void replay(int target);
		void updateAmplitudes(int target, int result);
		void playAltHistory(int target);
	public:
		//do some stuff to modification methods
		// modifications must either:
		//	a - update the entangled state or...
		//	b - break the entanglement
		EntangledRegister(MAX_UNSIGNED init, int width, Entanglement* ent);
		void apply2x2Matrix(int target, Matrix *m);
		int measure(int target, bool preserve);
		vector<Matrix>* getOpHistory(int target);
		void setAleph(bool aleph);
		bool getAleph();

		void pairMeasured(int target, int result);
};
}

#endif
