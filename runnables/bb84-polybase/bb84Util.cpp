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
ClassicRegister BB84Util::generateRandomDenseRegister(int width, bool allOnes, int density) {
	ClassicRegister r = ClassicRegister(BB84Util::REGISTER_SIZE, density-1);
	int i;
	for ( i = 0; i < BB84Util::REGISTER_SIZE; i++ ) {
		int bitValue = 1;
		if ( !allOnes ) {
			bitValue = abs(rand() % density);
		}
		r.setBit(i, bitValue);
	}
	return r;
}
 
Register BB84Util::encodeRegister(ClassicRegister bits, 
	ClassicRegister bases, int density) {
	Register r = Register((MAX_UNSIGNED)0, BB84Util::REGISTER_SIZE);

	SigmaX* sx = new SigmaX();
	RotateX* rx = new RotateX();

	int i;
	for ( i = 0; i < BB84Util::REGISTER_SIZE; i++ ) {
		if ( bits.getBit(i) == 1 ) {
			r.applyGate(sx, i);
		}
		float psiFactor = abs(bases.getBit(i) / ( density * 2 ));
		rx->setPsi(pi * psiFactor);
		r.applyGate(rx, i);
	}
	return r;
}

ClassicRegister BB84Util::decodeRegister(Register quBits,
			ClassicRegister bases, int density) {
	ClassicRegister bits = ClassicRegister(BB84Util::REGISTER_SIZE);
	RotateX* rx = new RotateX();
	int i;
	for ( i = 0; i < BB84Util::REGISTER_SIZE; i++ ) {
		float psiFactor = abs(bases.getBit(i) / ( density * 2 ));
		rx->setPsi(pi * psiFactor);
		quBits.applyGate(rx, i);
		bits.setBit(i, quBits.measure(i));
	}
	return bits;
}

ClassicRegister BB84Util::generateErrorBits(
	ClassicRegister rawKey, int ecBits) {
	int i;
	int j = 0;
	ClassicRegister errorBits = ClassicRegister(ecBits);
	for ( i = rawKey.getWidth() - ecBits; i < ecBits; i++ ) {
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
	for ( i = rawKey.getWidth() - errorBits.getWidth(); 
		i < rawKey.getWidth(); i++ ) {
		if ( rawKey.getBit(i) != errorBits.getBit(j) ) {
			failures++;
		}
		j++;
	}
	return (float)failures/(float)j;
}

void BB84Util::privacyAmplification(ClassicRegister rawKey, int ecBits) {
	char buffer [rawKey.getWidth() - ecBits +1];
	unsigned char finalKey[SHA512_DIGEST_LENGTH];
	int i;
	for ( i = 0; i < rawKey.getWidth() - ecBits; i++ ) {
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
