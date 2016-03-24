/*
 * bb84determiner_runnable.h
 */

#ifndef __QUANTUM_BB84_DETERMINER_RUNNABLE_H
#define __QUANTUM_BB84_DETERMINER_RUNNABLE_H

#include <string>
#include "iRunnable.h"

using namespace std;
using namespace Quantum;

namespace Quantum {

class BB84Determiner_Runnable : public iRunnable {
	public:
		virtual void Run();
};
}

#endif
