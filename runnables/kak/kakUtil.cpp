/*
 * bb84Util.h
 */
#include <memory>
#include <openssl/sha.h>

#include "classicRegister.h"
#include "gates.cpp"
#include "kakUtil.h"
#include "register.h"

using namespace std;
using namespace Quantum;

namespace KAK {
void KAKUtil::encodeRegister(Register &bits, float rotVal) {
	RotateX* rx =  new RotateX();
	int i;
	rx->setPsi(rotVal);

	for ( i = 0; i < bits.getWidth(); i++ ) {
		bits.applyGate(rx, i);
	}
}

void KAKUtil::privacyAmplification(ClassicRegister rawKey) {
	char buffer [KAKUtil::KEY_LENGTH+1];
	unsigned char finalKey[SHA512_DIGEST_LENGTH];

	int i;
	for ( i = 0; i < KAKUtil::KEY_LENGTH; i++ ) {
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
