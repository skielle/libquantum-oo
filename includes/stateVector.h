/*
 * stateVector.h
 */

#include <vector>

#include "matrix.h"
#include "__stub_remoteQubit.h"
#include "stateVectorOperation.h"
#include "channelService.h"

#ifndef __QUANTUM_STATE_VECTOR
#define __QUANTUM_STATE_VECTOR

using namespace std;
using namespace QuantumChannel;
namespace Quantum {
class Qubit;

class StateVector: public enable_shared_from_this<StateVector> {
	private:
		int index; 
		Matrix qsv;
	public:
		vector<StateVectorOperation> opHistory;
		vector<RemoteQubit> remoteQubits;
		StateVector();
		StateVector(int bitWidth);
		StateVector(Matrix m);
		void setIndex(int newIndex);
		int getIndex();
		void resize(int newSize);
		void applyOperation(Matrix operation, int position);
		void applyOperation(Matrix operation, int input1, int input2);
		void applyOperation(Matrix operation, vector<int> inputs);
		void applyOperation(Matrix operation, vector<int> inputs,
			bool addToHistory);
		void applyOperation(Matrix operation, 
			vector< shared_ptr<Qubit> > inputs);
		void getQubit(int position);
		double getAlpha(int position);
		double getBeta(int position);
		int measure(int position);
		int measure(int position, int forceResult);
		int measure(int position, int forceResult,
			bool propagate);
		void updateSystemsTable();
		void print();
		int getWidth();
		void reduce();
		void normalize();
		void sync();
		void replay();
		bool isEntangled(int position);
		static bool isBitSet(int index, int position);
		void swapBits(int position1, int position2);
		vector<int> generateRowMap(vector<int> inputs);
		Matrix toMatrix();
	friend grpc::Status ChannelService::SendMeasurementMessage(
		grpc::ServerContext* context,
		const QuantumMessage::MeasurementMessage* request,
		QuantumMessage::VoidMessage* reply);
};
}

#endif
