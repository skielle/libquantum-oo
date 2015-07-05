/*
 * classicRegister.cpp
 */

#include <sys/types.h>
#include <vector>

#include "classic.h"
#include "classicRegister.h"
#include "config.h"
#include "defs.h"
#include "error.h"
#include "quantumMessage.pb.h"

using namespace std;
using namespace Quantum;

namespace Quantum {
ClassicRegister::ClassicRegister(int initval, int width) {
	if ( Classic::ipow(2, width) < initval ){
		Error::error(QUANTUM_EINDEXOOB);
	}
	this->bitValue = initval;
	this->width = width;
}

void ClassicRegister::setBit(int index, int value) {
	if ( index > width ) {
		Error::error(QUANTUM_EINDEXOOB);
	}
	if ( this->getBit(index) != value ) {
		if ( value == 0 ) {
			this->bitValue -= Classic::ipow(2, index);
		} else {
			this->bitValue += Classic::ipow(2, index);
		}
	}
}

int ClassicRegister::getBit(int index) {
	return (this->bitValue >> index) % 2;
}

int ClassicRegister::getWidth() {
	return this->width;
}

void ClassicRegister::print() {
	int i;
	for ( i = this->width-1; i >= 0; i-- ) {
		printf("%i", (this->bitValue >> i) % 2 );
	}
	printf("\n");
}

QuantumMessage::ClassicRegisterMessage ClassicRegister::serialize() {
	QuantumMessage::ClassicRegisterMessage saveMessage;
	saveMessage.set_bitvalue(this->bitValue);
	saveMessage.set_width(this->width);
	return saveMessage;
}
ClassicRegister& ClassicRegister::unserialize(
	const QuantumMessage::ClassicRegisterMessage* loadMessage) {
	ClassicRegister* ret = new ClassicRegister(loadMessage->bitvalue(), loadMessage->width());
	return *ret;
}
}
