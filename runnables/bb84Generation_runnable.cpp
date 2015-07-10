/*
 * bb84Generation_runnable.cpp
 */

#include <unistd.h>

#include "bb84Generation_runnable.h"
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

void BB84Generation_Runnable::Run() {
	System* sys = System::getInstance();
	ClassicRegister rawKey = ClassicRegister(0, BB84Util::RAW_KEY_LENGTH);
	int keyBitsDetermined = 0;
	ClassicRegister bits = ClassicRegister(0, BB84Util::REGISTER_SIZE);
	ClassicRegister bases = ClassicRegister(0, BB84Util::REGISTER_SIZE);
	Register quBitReg = Register((MAX_UNSIGNED)0, BB84Util::REGISTER_SIZE);
	ClassicRegister correctBases = ClassicRegister(0, 
		BB84Util::REGISTER_SIZE );

	BB84Util::States runState = BB84Util::KEYEX_INITIATED;
 	
	QuantumChannel::ChannelService_client csc("127.0.0.1", 50085);

	while ( runState != BB84Util::KEYEX_COMPLETE && 
		keyBitsDetermined < BB84Util::RAW_KEY_LENGTH ) {
		switch ( runState ) { 
		case BB84Util::KEYEX_INITIATED:
			//TODO: wait for client connecting in
			bits = BB84Util::generateRandomClassicRegister(
					BB84Util::REGISTER_SIZE);
			printf("Bits: \t");
			bits.print();
			bases = BB84Util::generateRandomClassicRegister(
					BB84Util::REGISTER_SIZE);
			printf("Bases: \t");
			bases.print();
			quBitReg = BB84Util::encodeRegister(bits, bases);
		
			csc.SendRegister(quBitReg);
			runState = BB84Util::KEYEX_QBITS_SENT;
			break;
		case BB84Util::KEYEX_QBITS_SENT:
			while( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			if ( sys->getMessageType() == 			
				SystemMessage::CLASSIC_REGISTER_RECIEVED ) {
				shared_ptr<ClassicRegister> detBases =
					sys->getMessage<ClassicRegister>();
				printf("Agree:\t");
				int i;
				for ( i = 0; i < detBases->getWidth(); i++ ) {
					if ( bases.getBit(i) == 
						detBases->getBit(i) ) {
						rawKey.setBit(keyBitsDetermined,
							bits.getBit(i));
						keyBitsDetermined++;
						printf("^");
						correctBases.setBit(i, 1);
					} else {
						printf(" ");
					}
				}
				printf("\n");
				csc.SendClassicRegister(correctBases);
				runState = BB84Util::KEYEX_INITIATED;
			}
			break;
		}
	}
}
