/*
 * qubit.h
 */

#include <complex>
#include <memory>
#include <string>

#include "matrix.h"
#include "stateVector.h"
#include "quantumMessage.pb.h"
#include "remotePeer.h"

#ifndef __QUANTUM_BIT_H
#define __QUANTUM_BIT_H

using namespace std;

namespace Quantum {
class Qubit {
	private:
		Qubit();
		Qubit(bool isRemote);
		bool deleted;		
	public:
		shared_ptr<StateVector> v;
		int position;
		RemotePeerEntry origin;

		static shared_ptr<Qubit> create();
		void applyMatrix(Matrix m);
		void init();
		void guardDeleted();
		int measure();
		int measure(int forceValue);
		void print();
		void dereference();

		complex<double> getAlpha();
		complex<double> getBeta();

		QuantumMessage::QubitMessage serialize();
		static Qubit& unserialize(
			const QuantumMessage::QubitMessage* loadMessage);
};
}
#endif
