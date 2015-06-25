#include <stdio.h>
#include <fstream>
#include <math.h>
#include "../node.h"

using namespace std;
using namespace Quantum;

int main() {

	Node n(100, 0);

	n.setAmplitude(10);
	n.setState(5);

	printf("amplitude = %f, state = %i\n", n.getAmplitude(), 
		n.getState());

	return 0;
}
