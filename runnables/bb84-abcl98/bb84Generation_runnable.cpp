/*
 * bb84Generation_runnable.cpp
 */
#include <string>
#include <unistd.h>

#include "bb84Generation_runnable.h"
#include "bb84Util.h"
#include "channelListener.h"
#include "channelService_client.h"
#include "classicRegister.h"
#include "gates.cpp"
#include "register.h"
#include "system.h"
#include "systemMessage.h"

using namespace std;
using namespace Quantum;
using namespace BB84;

void BB84Generation_Runnable::Run() {
	System* sys = System::getInstance();
	ClassicRegister rawKey = ClassicRegister(BB84Util::RAW_KEY_LENGTH);
	ClassicRegister rawKeyBases = ClassicRegister(BB84Util::RAW_KEY_LENGTH);

	int keyBitsDetermined = 0;
	ClassicRegister bits = ClassicRegister(BB84Util::REGISTER_SIZE);
	ClassicRegister bases = ClassicRegister(BB84Util::REGISTER_SIZE);
	Register quBitReg = Register((MAX_UNSIGNED)0, BB84Util::REGISTER_SIZE);
	ClassicRegister correctBases = ClassicRegister(
		BB84Util::REGISTER_SIZE );

	BB84Util::States runState = BB84Util::KEYEX_INITIATED;
 	
	QuantumChannel::ChannelService_client csc("127.0.0.1", 50085);

	while ( runState != BB84Util::KEYEX_COMPLETE ) {
		sleep(1);
		switch ( runState ) { 
		case BB84Util::KEYEX_INITIATED:
			//TODO: wait for client connecting in
			bits = ClassicRegister::Random(
				BB84Util::REGISTER_SIZE);
			printf("Bits: \t");
			bits.print();
			bases = ClassicRegister::Random(
				BB84Util::REGISTER_SIZE, BB84Util::BASE_BIAS);
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
				ClassicRegister detBases =
					*(sys->getMessage<ClassicRegister>());
				printf("Agree:\t");
				int i;
				for ( i = BB84Util::REGISTER_SIZE-1;
					i >= 0 && keyBitsDetermined < 
					BB84Util::RAW_KEY_LENGTH; i-- ) {
					if ( bases.getBit(i) == 
						detBases.getBit(i) ) {
						rawKey.setBit(keyBitsDetermined,
							bits.getBit(i));
						rawKeyBases.setBit(
							keyBitsDetermined,
							bases.getBit(i));
						keyBitsDetermined++;
						printf("^");
						correctBases.setBit(i, 1);
					} else {
						correctBases.setBit(i, 0);
						printf(" ");
					}
				}
				printf("\n");
				csc.SendClassicRegister(correctBases);
				if ( keyBitsDetermined ==
					BB84Util::RAW_KEY_LENGTH ) {
					runState = BB84Util::KEYEX_EC;
				} else {
					runState = BB84Util::KEYEX_INITIATED;
				}
			}
			break;
		case BB84Util::KEYEX_EC:
			printf("Performing error checking, tolerance: %f\n", 
				BB84Util::ERROR_TOLERANCE);
			ClassicRegister errorBits =
				BB84Util::generateErrorBits(rawKey);
			csc.SendClassicRegister(errorBits);

			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			errorBits = *(sys->getMessage<ClassicRegister>());
			float errorRate_0 = 
				BB84Util::checkErrorBits(rawKey, errorBits, 	
						rawKeyBases, 0);
			float errorRate_1 = 
				BB84Util::checkErrorBits(rawKey, errorBits, 
						rawKeyBases, 1);
			if ( errorRate_0 >= BB84Util::ERROR_TOLERANCE 
				|| errorRate_1 >= BB84Util::ERROR_TOLERANCE ) {
				printf("Error rate of %f or %f above tolerance.\n",
					errorRate_0, errorRate_1);
				keyBitsDetermined = 0;
				rawKey = ClassicRegister(BB84Util::RAW_KEY_LENGTH);
				runState = BB84Util::KEYEX_INITIATED;
			} else {
				printf("Error rate of %f and %f within tolerance.\n",
					errorRate_0, errorRate_1);
				runState = BB84Util::KEYEX_COMPLETE;
			}
			break;
		}
	}
	BB84Util::privacyAmplification(rawKey);
	sys->stopServer();
}
