/*
 * kakParty_runnable.cpp
 */
#include <unistd.h>

#include "channelListener.h"
#include "channelService_client.h"
#include "classicRegister.h"
#include "gates.cpp"
#include "kakParty_runnable.h"
#include "kakUtil.h"
#include "register.h"
#include "system.h"

using namespace std;
using namespace Quantum;
using namespace KAK;

bool KAKParty_Runnable::isInstigator() {
	return this->_isInstigator;
}

void KAKParty_Runnable::setInstigator(bool instigate) {
	this->_isInstigator = instigate;
}

void KAKParty_Runnable::Run() {
	KAKUtil::States runState;
	int peerPort;
	int iKey = 0;
	System* sys = System::getInstance();

	Register r = Register((MAX_UNSIGNED)0, KAKUtil::REGISTER_SIZE);
	ClassicRegister cr = ClassicRegister(KAKUtil::REGISTER_SIZE);

	float psi = 1;

	ClassicRegister rawKey = ClassicRegister(KAKUtil::KEY_LENGTH);

	if ( this->isInstigator() ) {
		runState = KAKUtil::INSTIGATE;
		peerPort = 50107;
	} else {
		runState = KAKUtil::IDLE;
		peerPort = 50106;
	}

	QuantumChannel::ChannelService_client csc("127.0.0.1", peerPort);

	while ( runState != KAKUtil::KEYEX_COMPLETE ) {
		sleep(1);
		switch ( runState ) {
		case KAKUtil::IDLE:
			//wait for instigate message
			//

			//store reg value, rotate, send rotated register
			cr = ClassicRegister::Random(KAKUtil::REGISTER_SIZE);

			cr.print();

			int i;
			for ( i = 0; i < cr.getWidth() 
				&& iKey < rawKey.getWidth(); i++ ) {
				rawKey.setBit(iKey, cr.getBit(i));
				iKey++;
			}

			r = Register(&cr);
			KAKUtil::encodeRegister(r, psi);
			csc.SendRegister(r);			

			//transition to wait for mutually rotated register
			runState = KAKUtil::WAIT_FOR_MUROTREG;
			break;
		case KAKUtil::INSTIGATE:
			//send instigate message
			//transition to wait for rotated register
			runState = KAKUtil::WAIT_FOR_ROTREG;
			break;
		case KAKUtil::WAIT_FOR_ROTREG:
			//wait for rotated register
			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			//send mutually rotate register
			if ( sys->getMessageType() ==
				SystemMessage::REGISTER_RECIEVED ) {
				r = *(sys->getMessage<Register>());
				KAKUtil::encodeRegister(r, psi);
				csc.SendRegister(r);
			}
			//transition to wait for unrotated register
			runState = KAKUtil::WAIT_FOR_UNROTREG;
			break;
		case KAKUtil::WAIT_FOR_MUROTREG:
			//wait for mutually rotated register
			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			//send unrotated register
			if ( sys->getMessageType() ==
				SystemMessage::REGISTER_RECIEVED ) {
				r = *(sys->getMessage<Register>());
				KAKUtil::encodeRegister(r, -1 * psi);
				csc.SendRegister(r);
			}
			//check if that's the whole key
			if ( iKey == KAKUtil::KEY_LENGTH ) {
				//if whole key, transition to keyex complete
				runState = KAKUtil::KEYEX_COMPLETE;
			} else {
				//else transition to idle
				runState = KAKUtil::IDLE;
			}

			break;
		case KAKUtil::WAIT_FOR_UNROTREG:
			//wait for unrotated register
			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			if ( sys->getMessageType() ==
				SystemMessage::REGISTER_RECIEVED ) {
				r = *(sys->getMessage<Register>());
				//unrotate unrotated register
				ClassicRegister temp = 
					ClassicRegister(r.getWidth());
				KAKUtil::encodeRegister(r, -1 * psi);
				//add register to key
				for ( i = 0; i < r.getWidth() && 
					iKey < rawKey.getWidth(); i++ ) {
					int bitVal = r.measure(i);
					temp.setBit(i, bitVal);
					rawKey.setBit(iKey, bitVal);
					iKey++;
				}
				temp.print();
			}
			//check if that's the whole key
			if ( iKey == KAKUtil::KEY_LENGTH ) {
				//if whole key, transition to keyex complete
				runState = KAKUtil::KEYEX_COMPLETE;
			} else {
				//else transition to instigate
				runState = KAKUtil::INSTIGATE;
			}

			break;
		case KAKUtil::RESET:
			//send reset
			//clear key
			//transition to idle or instigate
			break;
		}
	}
	KAKUtil::privacyAmplification(rawKey);
	sys->stopServer();
}
