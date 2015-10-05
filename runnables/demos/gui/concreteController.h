/*
 * concreteController.h
 */
#include <memory>

#include "controllerListener.h"
#include "iController.h"
#include "register.h"

#ifndef __QUANTUMGUI_CONCRETECONTROLLER_H
#define __QUANTUMGUI_CONCRETECONTROLLER_H

namespace QuantumGUI {
using namespace Quantum;
class ConcreteController : public iController,
	public enable_shared_from_this<ConcreteController> {
	public:
		static shared_ptr<ConcreteController> create();
		void Run();
		void Process();
	private:
		Register modelRegister;
		ConcreteController();
};
}
#endif
