/*
 * entangledRegister.h
 */

#include <memory>
#include <vector>
#include "register.h"
#include "config.h"
#include "matrix.h"
#include "quantumMessage.pb.h"

#ifndef __QUANTUM_ENTANGLED_REGISTER_H
#define __QUANTUM_ENTANGLED_REGISTER_H

namespace Quantum {
class Entanglement;

class EntangledRegister: public Register {
	protected:
		shared_ptr<Entanglement> ent;
		vector<Matrix>** opHistory;
		bool _isAleph;

		void revert(int target);
		void replay(int target);
		void updateAmplitudes(int target, int result);
		void playAltHistory(int target);
	public:
		//do some stuff to modification methods
		// modifications must either:
		//	a - update the entangled state or...
		//	b - break the entanglement
		EntangledRegister(MAX_UNSIGNED init, int width, shared_ptr<Entanglement> ent);
		EntangledRegister(Matrix* m, int width, shared_ptr<Entanglement> ent);
		void apply2x2Matrix(int target, Matrix *m);
		int measure(int target, bool preserve);
		vector<Matrix>* getOpHistory(int target);
		void setAleph(bool aleph);
		bool isAleph();
		bool isEntangled(int target);

		shared_ptr<Entanglement> getEntanglement();
		void pairMeasured(int target, int result);

		QuantumMessage::EntangledRegisterMessage serialize();
		static shared_ptr<EntangledRegister> unserialize(
			const QuantumMessage::EntangledRegisterMessage* 
			loadMessage);
};
}

#endif
