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
		vector<int> bitValue;
		int width;
		int max;

	public:
		ClassicRegister(int width);
		ClassicRegister(int width, int _max);

		int getWidth();
		void setMax(int _max);
		int getMax();
		void setBit(int index, int value);
		int getBit(int index);

		void setValue(int regValue);
		int getValue();
		void print();

		QuantumMessage::ClassicRegisterMessage serialize();
		static ClassicRegister& unserialize(
			const QuantumMessage::ClassicRegisterMessage* loadMessage);
		static ClassicRegister& Random(int width);
		static ClassicRegister& Random(int width, float p0);
};

}

#endif
