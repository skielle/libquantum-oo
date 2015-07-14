/*
 * bb84Determination_runnable.cpp
 */
#include <string>
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

	ClassicRegister rawKey = ClassicRegister(BB84Util::RAW_KEY_LENGTH);
	int keyBitsDetermined = 0;
	ClassicRegister bits = ClassicRegister(BB84Util::REGISTER_SIZE);
	ClassicRegister bases = ClassicRegister(BB84Util::REGISTER_SIZE);
	ClassicRegister correctBases = ClassicRegister(
		BB84Util::REGISTER_SIZE );

	BB84Util::States runState = BB84Util::KEYEX_INITIATED;

	QuantumChannel::ChannelService_client csc("127.0.0.1", 50084);

	while ( runState != BB84Util::KEYEX_COMPLETE ) {
		switch ( runState ) {
		case BB84Util::KEYEX_INITIATED:
			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			if ( sys->getMessageType() == 
				SystemMessage::REGISTER_RECIEVED ) {
		
				bases = BB84Util::generateRandomClassicRegister(
					BB84Util::REGISTER_SIZE, false);
				Register quBitReg = 
					*(sys->getMessage<Register>());
				bits = BB84Util::decodeRegister( quBitReg,
					bases);
				printf("Bits: \t");
				bits.print();
				printf("Bases: \t");
				bases.print();

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
				ClassicRegister correctBases =
					*(sys->getMessage<ClassicRegister>());
				printf("Agree:\t");
				int i;
				for ( i = BB84Util::REGISTER_SIZE-1; i >= 0 &&
					keyBitsDetermined < 
					BB84Util::RAW_KEY_LENGTH; 
					i-- ) {
					if ( correctBases.getBit(i) == 1 ) {
						rawKey.setBit(keyBitsDetermined,
							bits.getBit(i));
						keyBitsDetermined++;
						printf("^");
					} else {
						printf(" ");
					}
				}
				printf("\n");
				if ( keyBitsDetermined 
					== BB84Util::RAW_KEY_LENGTH ) {
					runState = BB84Util::KEYEX_EC;
				} else {
					runState = BB84Util::KEYEX_INITIATED;
				}
			}
			break;
		case BB84Util::KEYEX_EC:
			printf("Performing error checking, tolerance: %f\n", 
				BB84Util::ERROR_TOLERANCE);
			csc.SendClassicRegister(
				BB84Util::generateErrorBits(rawKey));

			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			ClassicRegister errorBits =
				*(sys->getMessage<ClassicRegister>());
			float errorRate = 
				BB84Util::checkErrorBits(rawKey, errorBits);
			if ( errorRate > BB84Util::ERROR_TOLERANCE ) {
				printf("Error rate of %f above tolerance.\n",
					errorRate);
				keyBitsDetermined = 0;
				rawKey = ClassicRegister(BB84Util::RAW_KEY_LENGTH);
				runState = BB84Util::KEYEX_INITIATED;
			} else {
				printf("Error rate of %f within tolerance.\n",
					errorRate);
				runState = BB84Util::KEYEX_COMPLETE;
			}
			break;
		}
	}
	BB84Util::privacyAmplification(rawKey);
	terminate();
}
