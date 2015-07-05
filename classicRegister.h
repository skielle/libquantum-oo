/*
 * classicRegister.h
 */

#ifndef __QUANTUM_CLASSIC_REGISTER_H
#define __QUANTUM_CLASSIC_REGISTER_H

#include <sys/types.h>
#include <vector>

#include "config.h"
#include "defs.h"
#include "error.h"
#include "iRegister.h"
#include "quantumMessage.pb.h"

using namespace std;

namespace Quantum {
class ClassicRegister : public iRegister {
	protected:
		int bitValue;
		int width;

	public:
		ClassicRegister(int initval, int width);

		int getWidth();
		void setBit(int index, int value);
		int getBit(int index);

		void print();

		QuantumMessage::ClassicRegisterMessage serialize();
		static ClassicRegister& unserialize(
			const QuantumMessage::ClassicRegisterMessage* loadMessage);
};

}

#endif
