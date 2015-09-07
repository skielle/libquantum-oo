/*
 * bb84Util.h
 */
#include <memory>

#include "classicRegister.h"
#include "register.h"

#ifndef __BB84_UTIL_H
#define __BB84_UTIL_H

using namespace std;
using namespace Quantum;

namespace BB84 {
class BB84Util {
	public:
		enum States {
			INITIAL = 0,
			KEYEX_INITIATED = 1,
			KEYEX_QBITS_SENT = 2,
			KEYEX_EC = 3,
			KEYEX_CONFIG = 4,
			KEYEX_COMPLETE = 128
		};

		enum Signals {
			RESET = 0,
			DET_READY = 1,
			SENDING_QBITS = 2,
			SENDING_MBASIS = 3,
			SENDING_AGREE = 4
		};

		enum Config_Addresses {
			CA_NUMBASES = 0,
			CA_KEYLEN = 1,
			CA_ECBITS = 2,
			CA_ECTOL = 3
		};

		static constexpr float ERROR_TOLERANCE = .02;
		static const int REGISTER_SIZE = 16;
		static const int KEY_LENGTH = 64;
		static const int RAW_KEY_LENGTH = KEY_LENGTH * 1.5;

		static ClassicRegister generateRandomClassicRegister(int width,
			bool allOnes);
		static ClassicRegister generateRandomDenseRegister(int width,
			bool allOnes, int density);
		static Register encodeRegister(ClassicRegister bits,
			ClassicRegister basis, int density);
		static ClassicRegister decodeRegister(Register quBits,
			ClassicRegister basis, int density);
		static ClassicRegister generateErrorBits(
			ClassicRegister rawKey, int ecBits);
		static float checkErrorBits(ClassicRegister rawKey, 
			ClassicRegister errorBits);
		static void privacyAmplification(
			ClassicRegister rawKey, int ecBits);
};
}
#endif
