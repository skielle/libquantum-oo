#include "concreteController.h"

using namespace QuantumGUI;

int main() {
	shared_ptr<ConcreteController> c = ConcreteController::create();
	c->Run();
	return 0;
}
