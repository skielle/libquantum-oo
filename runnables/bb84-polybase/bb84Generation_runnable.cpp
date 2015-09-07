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
	ClassicRegister rawKey = ClassicRegister(this->keyLen + this->ecBits);
	int keyBitsDetermined = 0;
	ClassicRegister bits = ClassicRegister(BB84Util::REGISTER_SIZE);
	ClassicRegister bases = ClassicRegister(BB84Util::REGISTER_SIZE);
	Register quBitReg = Register((MAX_UNSIGNED)0, BB84Util::REGISTER_SIZE);
	ClassicRegister correctBases = ClassicRegister(
		BB84Util::REGISTER_SIZE );

	ClassicRegister config = ClassicRegister(8);
	config.setMax(INT_MAX);
	config.setBit(BB84Util::CA_NUMBASES, this->numBases);
	config.setBit(BB84Util::CA_KEYLEN, this->keyLen);
	config.setBit(BB84Util::CA_ECBITS, this->ecBits);
	config.setBit(BB84Util::CA_ECTOL, this->ecTol);

	BB84Util::States runState = BB84Util::KEYEX_CONFIG;

	QuantumChannel::ChannelService_client csc("127.0.0.1", 50085);
 	
	csc.SendClassicRegister(config);
	
	runState = BB84Util::KEYEX_INITIATED;

	while ( runState != BB84Util::KEYEX_COMPLETE ) {
		sleep(1);
		switch ( runState ) { 
		case BB84Util::KEYEX_INITIATED:
			//TODO: wait for client connecting in
			bits = BB84Util::generateRandomClassicRegister(
					BB84Util::REGISTER_SIZE, false);
			printf("Bits: \t");
			bits.print();
			bases = BB84Util::generateRandomDenseRegister(
					BB84Util::REGISTER_SIZE, false,
					this->numBases);
			printf("Bases: \t");
			bases.print();
			quBitReg = BB84Util::encodeRegister(bits, bases, 
				this->numBases);
		
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
					rawKey.getWidth(); i-- ) {
					if ( bases.getBit(i) == 
						detBases.getBit(i) ) {
						rawKey.setBit(keyBitsDetermined,
							bits.getBit(i));
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
				BB84Util::generateErrorBits(
					rawKey, this->ecBits));

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
				rawKey = ClassicRegister(this->keyLen);
				runState = BB84Util::KEYEX_INITIATED;
			} else {
				printf("Error rate of %f within tolerance.\n",
					errorRate);
				runState = BB84Util::KEYEX_COMPLETE;
			}
			break;
		}
	}
	BB84Util::privacyAmplification(rawKey, this->ecBits);
	sys->stopServer();
}
