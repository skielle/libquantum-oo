/*
 * bb84Util.h
 */
#include <memory>

#include "classicRegister.h"
#include "register.h"

#ifndef __KAK_UTIL_H
#define __KAK_UTIL_H

using namespace std;
using namespace Quantum;

namespace KAK {
class KAKUtil {
	public:
		enum States {
			IDLE = 0,
			INSTIGATE = 1,
			WAIT_FOR_ROTREG = 2,
			WAIT_FOR_MUROTREG = 3,
			WAIT_FOR_UNROTREG = 4,
			RESET = 127,
			KEYEX_COMPLETE = 128
		};

		static const int REGISTER_SIZE = 8; 
		static const int KEY_LENGTH = 64;

		static void encodeRegister(Register &bits,
			float rotVal);
		static void privacyAmplification(
			ClassicRegister rawKey);
};
}
#endif
