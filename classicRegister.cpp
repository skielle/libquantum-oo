/*
 * classicRegister.cpp
 */

#include <math.h>
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
	this->max = 1;
}

ClassicRegister::ClassicRegister(int width, int _max) {
	this->bitValue = vector<int> (width,0);
	this->max = _max;
}

void ClassicRegister::setBit(int index, int value) {
	if ( index > this->bitValue.size() ) {
		Error::error(QUANTUM_EINDEXOOB);
	}
	if ( value < this->max ) {
		this->bitValue.at(index) = value;
	} else {
		this->bitValue.at(index) = this->max;
	}
}

int ClassicRegister::getBit(int index) {
	return this->bitValue.at(index);
}

int ClassicRegister::getWidth() {
	return this->bitValue.size();
}

void ClassicRegister::setMax(int _max) {
	this->max = _max;
}

int ClassicRegister::getMax() {
	return this->max;
}

void ClassicRegister::setValue(int regValue) {
	int i;
	for ( i = 0; i < this->bitValue.size() 
			; i++ ) {
		this->bitValue.at(i) = abs((regValue >> i) % 2);
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
	int i;
	
	QuantumMessage::ClassicRegisterMessage saveMessage;
	saveMessage.set_width(this->getWidth());
	saveMessage.set_max(this->getMax());
	
	for ( i = 0; i < this->getWidth(); i++ ) {
		saveMessage.add_bitvalue(this->getBit(i));
	}
	
	return saveMessage;
}
ClassicRegister& ClassicRegister::unserialize(
	const QuantumMessage::ClassicRegisterMessage* loadMessage) {
	int i;
	int _max = 1;

	if ( loadMessage->max() > 0 ) {
		_max = loadMessage->max();
	}

	ClassicRegister* ret = new ClassicRegister(loadMessage->width(), _max);

	for ( i = 0; i < loadMessage->bitvalue_size(); i++ ) {
		ret->setBit(i, loadMessage->bitvalue(i));
	}
	return *ret;
}

ClassicRegister& ClassicRegister::Random(int width) {
	ClassicRegister* ret = new ClassicRegister(width);
	int i;
	for ( i = 0; i < width; i++ ) {
		ret->setBit( i, rand() % 2 );
	}
	return *ret;
}

ClassicRegister& ClassicRegister::Random(int width, float p0) {
	ClassicRegister* ret = new ClassicRegister(width);
	int i;
	float cutoff = RAND_MAX * p0;

	for ( i = 0; i < width; i++ ) {
		if ( rand() < cutoff ) {
			ret->setBit(i, 0);
		} else {
			ret->setBit(i, 1);
		}
	}
	return *ret;
}

}
