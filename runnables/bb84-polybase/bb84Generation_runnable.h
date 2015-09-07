/*
 * bb84Generation_runnable.h
 */

#ifndef __BB84_GENERATOR_H
#define __BB84_GENERATOR_H

#include "iRunnable.h"

using namespace Quantum;
using namespace std;

class BB84Generation_Runnable : public iRunnable {
	public:
		int numBases = 2;
		int keyLen = 64;
		int ecBits = keyLen * 1.5;
		int ecTol = ecBits * 0.2;

		virtual void Run(); 
};

#endif
