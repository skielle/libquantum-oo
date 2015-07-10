/*
 * bb84Util.cpp
 */

#include <memory>

#include "bb84Util.h"
#include "classicRegister.h"
#include "register.h"
#include "gates.cpp"

using namespace std;
using namespace Quantum;

namespace BB84 {
ClassicRegister BB84Util::generateRandomClassicRegister(int width, bool allOnes) {
	ClassicRegister r = ClassicRegister(BB84Util::REGISTER_SIZE);
	int i;
	for ( i = 0; i < BB84Util::REGISTER_SIZE; i++ ) {
		int bitValue = 1;
		if ( !allOnes ) {
			bitValue = rand() % 2;
		}
		r.setBit(i, bitValue);
	}
	return r;
}
 
Register BB84Util::encodeRegister(ClassicRegister bits, 
	ClassicRegister bases) {
	Register r = Register((MAX_UNSIGNED)0, BB84Util::REGISTER_SIZE);

	SigmaX* sx = new SigmaX();
	Hadamard* h = new Hadamard();

	int i;
	for ( i = 0; i < BB84Util::REGISTER_SIZE; i++ ) {
		if ( bits.getBit(i) == 1 ) {
			r.applyGate(sx, i);
		}
		if ( bases.getBit(i) == 1 ) {
			r.applyGate(h, i);
		}
	}
	return r;
}

ClassicRegister BB84Util::decodeRegister(Register quBits,
			ClassicRegister bases) {
	ClassicRegister bits = ClassicRegister(BB84Util::REGISTER_SIZE);
	Hadamard* h = new Hadamard();
	int i;
	for ( i = 0; i < BB84Util::REGISTER_SIZE; i++ ) {
		if ( bases.getBit(i) == 1 ) {
			quBits.applyGate(h, i);
		}
		bits.setBit(i, quBits.measure(i));
	}
	return bits;
}
}
