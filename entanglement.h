/*
 * entanglement.h
 */
#ifndef __QUANTUM_ENTANGLEMENT_H
#define __QUANTUM_ENTANGLEMENT_H

#include <memory.h>

#include "channelService.h"
#include "channelService_client.h"
#include "entangledRegister.h"
#include "entangledPair.h"
#include "register.h"
#include "stub__networkRegister.h"

using namespace std;

namespace Quantum {
class Entanglement: public enable_shared_from_this<Entanglement> {
	protected:
		EntangledRegister* aleph;
		EntangledRegister* beit;
		bool isAleph__stub = false;
		bool isBeit__stub = false;
		EntangledPair* entanglements;

		void setAleph(EntangledRegister* _aleph);
		void setBeit(EntangledRegister* _beit);

		void makeAlephRemote();
		void makeBeitRemote();
	public:
		static Entanglement createEntanglement(MAX_UNSIGNED init, 
			int width);
		EntangledRegister* getAleph();
		EntangledRegister* getBeit();
		EntangledPair getEntanglement(int target);
		bool isEntangled( int target);

//remote consequences
		void entangle(int target, EntangledPair entanglementData);
		void measured(bool isAleph, int target, int result);

	friend EntangledRegister& EntangledRegister::unserialize(
	        const QuantumMessage::EntangledRegisterMessage* loadMessage);
	friend grpc::Status 
		QuantumChannel::ChannelService::SendEntangledRegister(
		grpc::ServerContext* context,
        	const QuantumMessage::EntangledRegisterMessage* request,
        	QuantumMessage::VoidMessage* reply); 
};
}
#endif		
