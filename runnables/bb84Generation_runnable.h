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
		virtual void Run(); 
};

#endif
