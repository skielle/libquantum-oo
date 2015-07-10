/*
 * bb84Determination_runnable.cpp
 */

#include <unistd.h>

#include "bb84Determination_runnable.h"
#include "bb84Util.h"
#include "channelListener.h"
#include "channelService_client.h"
#include "classicRegister.h"
#include "gates.cpp"
#include "register.h"
#include "system.h"

using namespace std;
using namespace Quantum;
using namespace BB84;

void BB84Determination_Runnable::Run() {
	System* sys = System::getInstance();

	ClassicRegister rawKey = ClassicRegister(0, BB84Util::RAW_KEY_LENGTH);
	int keyBitsDetermined = 0;
	ClassicRegister bits = ClassicRegister(0, BB84Util::REGISTER_SIZE);
	ClassicRegister bases = ClassicRegister(0, BB84Util::REGISTER_SIZE);
	ClassicRegister correctBases = ClassicRegister(0,
		BB84Util::REGISTER_SIZE );

	BB84Util::States runState = BB84Util::KEYEX_INITIATED;

	QuantumChannel::ChannelService_client csc("127.0.0.1", 50084);

	while ( runState != BB84Util::KEYEX_COMPLETE &&
		keyBitsDetermined < BB84Util::RAW_KEY_LENGTH ) {
		switch ( runState ) {
		case BB84Util::KEYEX_INITIATED:
			bases = BB84Util::generateRandomClassicRegister(
				BB84Util::REGISTER_SIZE);
			printf("Bases: \t");
			bases.print();
		
			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			if ( sys->getMessageType() == 
				SystemMessage::REGISTER_RECIEVED ) {
				shared_ptr<Register> quBitReg = 
					sys->getMessage<Register>();
				bits = BB84Util::decodeRegister( quBitReg,
					bases);
				bits.print();

				csc.SendClassicRegister(bases);
				runState = BB84Util::KEYEX_QBITS_SENT;
			}
			break;
		case BB84Util::KEYEX_QBITS_SENT:
			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			if ( sys->getMessageType() == 
				SystemMessage::CLASSIC_REGISTER_RECIEVED ) {
				shared_ptr<ClassicRegister> correctBases =
					sys->getMessage<ClassicRegister>();
				printf("Agree:\t");
				int i;
				for ( i = 0; i < correctBases->getWidth(); 
					i++ ) {
					if ( correctBases->getBit(i) == 1 ) {
						rawKey.setBit(keyBitsDetermined,
							bits.getBit(i));
						keyBitsDetermined++;
						printf("^");
					} else {
						printf(" ");
					}
				}
				printf("\n");
				runState = BB84Util::KEYEX_INITIATED;
			}
			break;
		}
	}
}
