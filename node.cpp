/*
 * node.cpp
 */
#include "node.h"
#include "config.h"

namespace Quantum {
Node::Node ( COMPLEX_FLOAT amplitude, MAX_UNSIGNED state ) {
	this->amplitude = amplitude;
	this->state = state;
}

void Node::setAmplitude ( COMPLEX_FLOAT amplitude ) {
	this->amplitude = amplitude; }

COMPLEX_FLOAT Node::getAmplitude () { return this->amplitude; }

void Node::setState ( MAX_UNSIGNED state ) { this->state = state; }

MAX_UNSIGNED Node::getState() { return this->state; }
}