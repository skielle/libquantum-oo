/*
 * systemMessage.cpp
 */
#include "systemMessage.h"

namespace Quantum {
SystemMessage::SystemMessage() {
	this->_messageType = SystemMessage::DEFAULT_MESSAGE;
}

SystemMessage::SystemMessage(int MessageType) {
	this->_messageType = MessageType;
}

int SystemMessage::getMessageType() {
	return this->_messageType;
}

void SystemMessage::setMessageType(int MessageType) {
	this->_messageType = MessageType;
}
bool SystemMessage::operator ==(SystemMessage &sm) {
	return this->_messageType == sm.getMessageType();
}
bool SystemMessage::operator ==(int smt) {
	return this->_messageType == smt;
}
}
