#include "superpositionController.h"

using namespace QuantumGUI;

int main() {
	shared_ptr<SuperpositionController> c = SuperpositionController::create();
	c->Run();
	return 0;
}
