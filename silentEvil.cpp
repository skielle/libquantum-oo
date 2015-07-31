/*
 * echoRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <stdio.h>

#include "classicRegister.h"
#include "silentEvil.h"
#include "register.h"
#include "system.h"
#include "systemMessage.h"

using namespace std;

namespace Quantum {
void SilentEvil::doEvil(shared_ptr<iRegister> reg, SystemMessage message) {
	System* sys = System::getInstance();

	if ( message == SystemMessage::REGISTER_RECIEVED ) {
		shared_ptr<Register> quBitReg = dynamic_pointer_cast<Register>(reg);
		printf("Evil:\t");
		int i;
		for ( i = 0; i < quBitReg->getWidth(); i++ ) {
			float evilFactor = rand() / (float)RAND_MAX;
			if ( evilFactor <= evilness ) {
				printf("$");
				quBitReg->measure(i);
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

void SilentEvil::setEvilness(float newEvilness) {
	this->evilness = newEvilness;
}

float SilentEvil::getEvilness() {
	return this->evilness;
}

}
