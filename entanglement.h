/*
 * entanglement.h
 */
#ifndef __QUANTUM_ENTANGLEMENT_H
#define __QUANTUM_ENTANGLEMENT_H

#include <memory.h>

#include "channelService_client.h"
#include "channelService.h"
#include "entangledRegister.h"
#include "entangledPair.h"
#include "quantumMessage.pb.h"
#include "register.h"
#include "stub__networkRegister.h"

using namespace std;

namespace Quantum {
class Entanglement: public enable_shared_from_this<Entanglement> {
	protected:
		shared_ptr<EntangledRegister> aleph;
		shared_ptr<EntangledRegister> beit;
		bool isAleph__stub = false;
		bool isBeit__stub = false;
		shared_ptr<QuantumChannel::ChannelService_client> aleph__stub;
		shared_ptr<QuantumChannel::ChannelService_client> beit__stub;
		EntangledPair* entanglements;

		void setAleph(shared_ptr<EntangledRegister> _aleph);
		void setBeit(shared_ptr<EntangledRegister> _beit);

		void makeAlephRemote(int css);
		void makeBeitRemote(int css);
		void makeAlephRemote(
			shared_ptr<QuantumChannel::ChannelService_client> csc);
		void makeBeitRemote(
			shared_ptr<QuantumChannel::ChannelService_client> csc);

		QuantumMessage::EntangledMeasurementMessage 
		createMeasurementMessage(
			bool _isAleph, int target, int result, 
			vector<Matrix>* targetOpHistory);
	public:
		static Entanglement createEntanglement(MAX_UNSIGNED init, 
			int width);
		shared_ptr<EntangledRegister> getAleph();
		shared_ptr<EntangledRegister> getBeit();
		EntangledPair getEntanglement(int target);
		bool isEntangled( int target);

//remote consequences
		void entangle(int target, EntangledPair entanglementData);
		void measured(bool isAleph, int target, int result);

	friend shared_ptr<EntangledRegister> EntangledRegister::unserialize(
	        const QuantumMessage::EntangledRegisterMessage* loadMessage);
	friend grpc::Status 
		QuantumChannel::ChannelService::SendEntangledRegister(
		grpc::ServerContext* context,
        	const QuantumMessage::EntangledRegisterMessage* request,
        	QuantumMessage::InetAddr* reply);
	friend bool 
		QuantumChannel::ChannelService_client::SendEntangledRegister(
		EntangledRegister r);
};
}
#endif		
