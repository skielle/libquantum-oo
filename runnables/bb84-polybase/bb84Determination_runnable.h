/*
 * bb84Determination_runnable.h
 */

#include "iRunnable.h"

using namespace Quantum;
using namespace std;

class BB84Determination_Runnable : public iRunnable {
	public:
		int numBases = 2;
		int keyLen = 64;
		int ecBits = keyLen * 1.5;
		int ecTol = ecBits * 0.2;

		virtual void Run();
};
