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
ClassicRegister::ClassicRegister(int width) {
	this->bitValue = vector<int> (width,0);
}

void ClassicRegister::setBit(int index, int value) {
	if ( index > this->bitValue.size() ) {
		Error::error(QUANTUM_EINDEXOOB);
	}
	if ( value == 0 ) {
		this->bitValue.at(index) = 0;
	} else {
		this->bitValue.at(index) = 1;
	}
}

int ClassicRegister::getBit(int index) {
	return this->bitValue.at(index);
}

int ClassicRegister::getWidth() {
	return this->bitValue.size();
}

void ClassicRegister::setValue(int regValue) {
	int i;
	for ( i = 0; i < this->bitValue.size() 
			; i++ ) {
		this->bitValue.at(i) = (regValue >> i) % 2;
	}
}

int ClassicRegister::getValue() {
	int value = 0;
	int i;
	for ( i = 0; i < this->bitValue.size(); i++ ) {
		if ( this->bitValue.at(i) == 1 ) {
			value += Classic::ipow( 2, this->bitValue.at(i) * i);
		}
	}
	return value;
} 

void ClassicRegister::print() {
	int i;
	int j = 0;
	int value = 0;
	for ( i = this->bitValue.size()-1; i >= 0; i-- ) {
		printf("%i", this->bitValue.at(i) );
		j++;
		if ( j % 16 == 0 && i != 0) {
			value = 0;
			printf("\n", value);
		}
	}
	printf("\n");
}

QuantumMessage::ClassicRegisterMessage ClassicRegister::serialize() {
	QuantumMessage::ClassicRegisterMessage saveMessage;
	saveMessage.set_bitvalue(this->getValue());
	saveMessage.set_width(this->bitValue.size());
	return saveMessage;
}
ClassicRegister& ClassicRegister::unserialize(
	const QuantumMessage::ClassicRegisterMessage* loadMessage) {
	ClassicRegister* ret = new ClassicRegister(loadMessage->width());
	ret->setValue(loadMessage->bitvalue());
	return *ret;
}
}
