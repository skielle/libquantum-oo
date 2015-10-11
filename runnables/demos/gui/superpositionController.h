/*
 * superpositionController.h
 */
#include <memory>

#include "controllerListener.h"
#include "iController.h"
#include "register.h"

#ifndef __QUANTUMGUI_SUPERPOSITIONCONTROLLER_H
#define __QUANTUMGUI_SUPERPOSITIONCONTROLLER_H

namespace QuantumGUI {
using namespace Quantum;
class SuperpositionController : public iController,
	public enable_shared_from_this<SuperpositionController> {
	public:
		static shared_ptr<SuperpositionController> create();
		void Run();
		void Process();
	private:
		Register modelRegister;
		SuperpositionController();
};
}
#endif
