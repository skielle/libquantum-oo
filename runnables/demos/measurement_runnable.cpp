/*
 * measurement_runnable.cpp
 */
#include <string>
#include <unistd.h>
#include <iostream>

#include "measurement_runnable.h"
#include "gates.cpp"
#include "register.h"
#include "system.h"
#include "systemMessage.h"

using namespace std;
using namespace Quantum;

void Measurement_Runnable::Run() {
	System* sys = System::getInstance();
	int bitWidth = 4;

	while ( true ) {
		Register* r = new Register((MAX_UNSIGNED)0, bitWidth);
		RotateX* rX = new RotateX();

		int degrees = 0;

		cout<<"Created new register:"<<endl;
		r->print();

		for ( int i = 0; i < bitWidth; i++ ) {
			cout<<"Enter polarization for bit "<<i
				<<" in degrees."<<endl;
			cin>>degrees;
			rX->setPsi(pi * degrees / 180);
			r->applyGate(rX, i);
		}
		cout<<"Resulting state:"<<endl;
		r->print();

		cout<<"Measuring:"<<endl;
		
		for ( int i = 0; i < bitWidth; i++ ) {
			r->measure(i);
			r->print();
		}
	}
	sys->stopServer();
}
