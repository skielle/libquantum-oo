/*
 * stub__networkRegister.h
 */

#ifndef __QUANTUM_STUB__NETWORKREGISTER_H
#define __QUANTUM_STUB__NETWORKREGISTER_H

namespace Quantum {
class stub__networkRegister {
	protected:
		Entanglement* ent;
		bool aleph;

	public:
		stub_NetworkRegister(MAX_UNSIGNED init, int width,
			Entanglement* ent);
		vector<Matrix>* getOpHistory(int target);
		void setAleph(bool aleph);
		bool getAleph();

		void pairMeasured(int target, int result);
};
}
#endif
