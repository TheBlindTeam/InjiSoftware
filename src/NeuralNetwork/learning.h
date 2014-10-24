#include "neuronsUtils.h"

void NInitBinary(NetworkSet *nWorkSet, Network* (*NInit)(int, int),
	int input, int output);

int NDichotomicLearn(NetworkSet *nWorkSet);

void NChangeLostNetwork(Network won,double wError, Network lost, double lError);

int NBackPropLearn(NetworkSet *nWorkSet);
