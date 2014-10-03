#include "neuronsUtils.h"

Network NGetTrainedNetwork(ExempleSet exSet, double maxError);

void NChangeLostNetwork(Network won, double wError, Network lost, double lError);
