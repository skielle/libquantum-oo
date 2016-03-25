#include "channelService_client.h"
#include "qubit.h"

using namespace Quantum;

int main() {
	QuantumChannel::ChannelService_client csc("127.0.0.1", 50000);
	double theta = M_PI / 4.0;
	double omega = M_PI / 45.0;

	printf("Initial rotation by %f, error rotation of %f\r\n", theta, omega);

	shared_ptr<Qubit> L1 = Qubit::create();
	shared_ptr<Qubit> L2 = Qubit::create();

	Matrix kakRotate = Matrix(2,2);
	kakRotate.set(0, 0, cos(theta));
	kakRotate.set(0, 1, sin(theta));
	kakRotate.set(1, 0, -sin(theta));
	kakRotate.set(1, 1, cos(theta));

	L1->applyMatrix(kakRotate);
	L1->print();

	Matrix CNOT = Matrix(4,4);
	CNOT.set(0, 0, 1);
	CNOT.set(1, 1, 1);
	CNOT.set(2, 3, 1);
	CNOT.set(3, 2, 1);

	vector< shared_ptr<Qubit> > inputs(2);

	inputs.at(0) = L1;
	inputs.at(1) = L2;
	L1->v->applyOperation(CNOT, inputs);

	L1->print();
	L2->print();

	csc.SendQubit(L2);
	csc.SendQubit(L1);
}
