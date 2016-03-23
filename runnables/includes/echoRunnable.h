/*
 * echoRunnable.h
 */
#ifndef __QUANTUM_ECHORUNNABLE_H
#define __QUANTUM_ECHORUNNABLE_H

#include "iRunnable.h"

namespace Quantum {
class EchoRunnable : public iRunnable {
	public:
		virtual void Run(); 
};
}
#endif
