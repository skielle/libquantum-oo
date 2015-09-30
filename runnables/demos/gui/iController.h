/*
 * iController.h
 */

#include "controllerListenerService.h"

#ifndef __QUANTUMGUI_ICONTROLLER_H
#define __QUANTUMGUI_ICONTROLLER_H

namespace QuantumGUI {

class iController {
	protected:
		shared_ptr<ControllerListenerService> service;
	public:
		virtual void Process() = 0;
		void setService(shared_ptr<ControllerListenerService> inService) {
			this->service = inService;
		}
};
}
#endif
