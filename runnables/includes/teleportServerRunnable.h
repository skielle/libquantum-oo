/*
 * teleportServerRunnable.h
 */
#ifndef __QUANTUM_TELEPORTRUNNABLE_H
#define __QUANTUM_TELEPORTRUNNABLE_H

#include "iRunnable.h"

namespace Quantum {
class TeleportServerRunnable : public iRunnable {
	public:
		virtual void Run(); 
};
}
#endif
