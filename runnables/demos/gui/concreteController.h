/*
 * concreteController.h
 */
#include <memory>

#include "controllerListener.h"
#include "iController.h"

#ifndef __QUANTUMGUI_CONCRETECONTROLLER_H
#define __QUANTUMGUI_CONCRETECONTROLLER_H

namespace QuantumGUI {

class ConcreteController : public iController,
	public enable_shared_from_this<ConcreteController> {
	public:
		static shared_ptr<ConcreteController> create();
		void Run();
		void Process();
	private:
		ConcreteController() {}
};
}
#endif
