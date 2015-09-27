/*
 * register.h
 */

#ifndef __QUANTUM_REGISTER_H
#define __QUANTUM_REGISTER_H

#include <sys/types.h>
#include <vector>

#include "classicRegister.h"
#include "config.h"
#include "defs.h"
#include "error.h"
#include "iRegister.h"
#include "matrix.h"
#include "qubit.h"
#include "quantumMessage.pb.h"

using namespace std;

namespace Quantum {
class Gate;

class Register : public iRegister {
	private:
		void fromClassicRegister(ClassicRegister *cr);
		void fromMatrix(Matrix *m);

	protected:
		vector<QuBit*> qubits;

	public:
		Register(Matrix *m, int width);
		Register(Matrix *m);
		Register(MAX_UNSIGNED initval, int width);
		Register(ClassicRegister *cr);
		Register();

		int getWidth();

		Register& copy();
		Matrix toMatrix();
		void applyGate(Gate* g, int target);
		void applyMatrix(int target, Matrix *m);
		void virtual apply2x2Matrix(int target, Matrix *m);
		int measure();
		int measure(int target);
		int virtual measure(int target, bool preserve);

		void print();
		void printSystem();

		QuantumMessage::RegisterMessage serialize();
		static Register& unserialize(
			const QuantumMessage::RegisterMessage* loadMessage);

		//void might not be right here, we might want to drop a qureg
		void stateCollapse(int bit, int value);
};

class Gate {
	public:
                virtual void run ( Register* r, int target ) = 0;
};

}

#endif
