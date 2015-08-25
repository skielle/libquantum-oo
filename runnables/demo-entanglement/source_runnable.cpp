/*
 * kakParty_runnable.cpp
 */
#include <unistd.h>

#include "channelListener.h"
#include "channelService_client.h"
#include "classicRegister.h"
#include "entangledPair.h"
#include "entangledRegister.h"
#include "entanglement.h"
#include "gates.cpp"
#include "source_runnable.h"
#include "register.h"
#include "system.h"

using namespace std;
using namespace Quantum;

void Source_Runnable::Run() {
	System* sys = System::getInstance();

	QuantumChannel::ChannelService_client csc("127.0.0.1", 50100);

	Entanglement e = Entanglement::createEntanglement((MAX_UNSIGNED) 0, 4);
	shared_ptr<EntangledRegister> a = e.getAleph();
	shared_ptr<EntangledRegister> b = e.getBeit();

	EntangledPair* ab0 = new EntangledPair(1/sqrt(2), .5, 0, .5);

	RotateX* rx = new RotateX();
	rx->setPsi(2*pi/4);

	a->EntangledRegister::applyGate(new Hadamard(), 1);
	b->EntangledRegister::applyGate(new Hadamard(), 1);

	e.entangle(1, *ab0);

	a->EntangledRegister::applyGate(rx, 1);

	csc.SendEntangledRegister(*b);
sleep(10);
	a->measure(1, true);
	a->print();
//	sys->stopServer();
}
