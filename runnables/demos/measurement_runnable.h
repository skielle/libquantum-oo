/*
 * measurement_runnable.h
 */

#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

#include "iRunnable.h"

using namespace Quantum;
using namespace std;

class Measurement_Runnable : public iRunnable {
	private:
	public:
		virtual void Run(); 
};

#endif
