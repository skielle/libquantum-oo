/*
 * source_runnable.h
 */

#ifndef __SOURCE_H
#define __SOURCE_H

#include "iRunnable.h"

using namespace Quantum;
using namespace std;

class Source_Runnable : public iRunnable {
	private:
	public:
		virtual void Run(); 
};

#endif
