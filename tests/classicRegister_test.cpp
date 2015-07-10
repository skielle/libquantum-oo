#include <stdio.h>
#include "../quantumMessage.pb.h"
#include "../classicRegister.h"

using namespace std;
using namespace Quantum;

int main() {
	ClassicRegister a = ClassicRegister(8);
	a.setValue(7);

	a.setBit(1, 0);

	a.print();

	QuantumMessage::ClassicRegisterMessage aS = a.serialize();
	ClassicRegister b = ClassicRegister::unserialize(&aS);

	b.print();

	return 0;
}
