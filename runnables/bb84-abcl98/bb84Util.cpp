/*
 * bb84Util.cpp
 */

#include <iostream>
#include <memory>
#include <openssl/sha.h>
#include <string.h>

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

ClassicRegister BB84Util::generateErrorBits(ClassicRegister rawKey) {
	int i;
	int j = 0;
	ClassicRegister errorBits = ClassicRegister(BB84Util::RAW_KEY_LENGTH -
		BB84Util::KEY_LENGTH );
	for ( i = BB84Util::KEY_LENGTH; i < BB84Util::RAW_KEY_LENGTH; i++ ) {
		errorBits.setBit(j, rawKey.getBit(i));
		j++;
	}
	return errorBits;
}

float BB84Util::checkErrorBits(ClassicRegister rawKey, 
	ClassicRegister errorBits) {
	int i;
	int j = 0;
	int failures = 0;
	for ( i = BB84Util::KEY_LENGTH; i < BB84Util::RAW_KEY_LENGTH; i++ ) {
		if ( rawKey.getBit(i) != errorBits.getBit(j) ) {
			failures++;
		}
		j++;
	}
	return (float)failures/(float)j;
}

float BB84Util::checkErrorBits(ClassicRegister rawKey, 
	ClassicRegister errorBits, ClassicRegister rawKeyBases, int baseValue) {
	int i;
	int j = 0;
	int tested = 0;
	int failures = 0;
	for ( i = BB84Util::KEY_LENGTH; i < BB84Util::RAW_KEY_LENGTH; i++ ) {
		if ( rawKeyBases.getBit(i) == baseValue ) {
			tested++;
			if ( rawKey.getBit(i) != errorBits.getBit(j) ) {
				failures++;
			}
		}
		j++;
	}
	if ( tested == 0 ) {
		return 0;
	} else {
		return (float)failures/(float)tested;
	}
}

void BB84Util::privacyAmplification(ClassicRegister rawKey) {
	char buffer [BB84Util::KEY_LENGTH+1];
	unsigned char finalKey[SHA512_DIGEST_LENGTH];
	int i;
	for ( i = 0; i < BB84Util::KEY_LENGTH; i++ ) {
		sprintf(&buffer[i], "%i", rawKey.getBit(i));
	}

	SHA512(reinterpret_cast<const unsigned char*>(buffer),
		 sizeof(buffer), finalKey);
	for ( i = 0; i < sizeof(finalKey); i++ ) {
		printf("%02x", finalKey[i] & 0xff);
	}
	printf("\n");
}

}
