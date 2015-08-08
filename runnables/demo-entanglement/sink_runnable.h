/*
 * sink_runnable.h
 */

#ifndef __SINK_H
#define __SINK_H

#include "iRunnable.h"

using namespace Quantum;
using namespace std;

class Sink_Runnable : public iRunnable {
	private:
	public:
		virtual void Run(); 
};

#endif
