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
#include "systemMessage.h"

using namespace std;
using namespace Quantum;
using namespace BB84;

void BB84Determination_Runnable::Run() {
	System* sys = System::getInstance();

	ClassicRegister rawKey = ClassicRegister(0);
	int keyBitsDetermined = 0;
	ClassicRegister bits = ClassicRegister(BB84Util::REGISTER_SIZE);
	ClassicRegister bases = ClassicRegister(BB84Util::REGISTER_SIZE);
	ClassicRegister config = ClassicRegister(BB84Util::REGISTER_SIZE);
	ClassicRegister correctBases = ClassicRegister(
		BB84Util::REGISTER_SIZE );

	BB84Util::States runState = BB84Util::KEYEX_CONFIG;

	config.setMax(INT_MAX);
	QuantumChannel::ChannelService_client csc("127.0.0.1", 50084);

	while ( runState != BB84Util::KEYEX_COMPLETE ) {
		switch ( runState ) {
		case BB84Util::KEYEX_CONFIG:
			while ( sys->isMessageQueueEmpty() ) {
			}
			if ( sys->getMessageType() == 
				SystemMessage::CLASSIC_REGISTER_RECIEVED ) {
				config = *(sys->getMessage<ClassicRegister>() );
			
				this->numBases =
					config.getBit(BB84Util::CA_NUMBASES);
				bases.setMax(this->numBases);
				
				this->keyLen =
					config.getBit(BB84Util::CA_KEYLEN);
				this->ecBits =
					config.getBit(BB84Util::CA_ECBITS);
				this->ecTol =
					config.getBit(BB84Util::CA_ECTOL);
				rawKey = ClassicRegister(
					this->keyLen + this->ecBits);
			}
			if ( this->numBases != 0 ) {
				runState = BB84Util::KEYEX_INITIATED;
			}				

			break;
		case BB84Util::KEYEX_INITIATED:
			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			if ( sys->getMessageType() == 
				SystemMessage::REGISTER_RECIEVED ) {
		
				bases = BB84Util::generateRandomDenseRegister(
					BB84Util::REGISTER_SIZE, false, this->numBases);
				Register quBitReg = 
					*(sys->getMessage<Register>());
				bits = BB84Util::decodeRegister( quBitReg,
					bases, this->numBases);
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
					keyBitsDetermined < rawKey.getWidth(); 
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
				if ( keyBitsDetermined == rawKey.getWidth() ) {
					runState = BB84Util::KEYEX_EC;
				} else {
					runState = BB84Util::KEYEX_INITIATED;
				}
			}
			break;
		case BB84Util::KEYEX_EC:
			printf("Performing error checking, tolerance: %f\n", 
				(float)this->ecTol / (float)this->ecBits);
			csc.SendClassicRegister(
				BB84Util::generateErrorBits(rawKey, 
				this->ecBits));

			while ( sys->isMessageQueueEmpty() ) {
				sleep(1);
			}
			ClassicRegister errorBits =
				*(sys->getMessage<ClassicRegister>());
			float errorRate = 
				BB84Util::checkErrorBits(rawKey, errorBits);
			if ( errorRate >= 
				(float)this->ecTol / (float)this->ecBits ) {
				printf("Error rate of %f above tolerance.\n",
					errorRate);
				keyBitsDetermined = 0;
				rawKey = ClassicRegister(rawKey.getWidth());
				runState = BB84Util::KEYEX_INITIATED;
			} else {
				printf("Error rate of %f within tolerance.\n",
					errorRate);
				runState = BB84Util::KEYEX_COMPLETE;
			}
			break;
		}
	}
	BB84Util::privacyAmplification(rawKey, ecBits);
	sys->stopServer();
}
