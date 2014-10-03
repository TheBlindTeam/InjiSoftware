#include "learning.h"
#include <time.h>
#include <stdlib.h>

Network NGetTrainedNetwork(ExempleSet exSet, double maxError)
{
	Network nWorks[2];
	double errors[2];
	srand(time(NULL));
	for (int i = 0; i < 2; i ++)
	{
		nWorks[i] = NInitializeCompleteNetwork(exSet);
		errors[i] = NComputeError(nWorks[i], exSet, 1);
	}do
	{
		if (errors[0] > errors[1])
			NChangeLostNetwork(nWorks[1], errors[1], nWorks[0], errors[0]);
		else
			NChangeLostNetwork(nWorks[0], errors[0], nWorks[1], errors[1]);
	 	for (int i = 0; i < 2; i ++)
			errors[i] = NComputeError(nWorks[i], exSet, 1);
	}while(errors[0] > maxError && errors[1] > maxError);
	if (errors[0] < maxError)
		return nWorks[0];
	return nWorks[1];
}

void NChangeLostNetwork(Network won, double wError, Network lost, double lError)
{
	double rdm;
	for (int i = 0; i < lost.nbLayers; i ++)
	{
		for (int j = 0; j < lost.layersSize[i]; j ++)
		{
			for (int k = 0; k < lost.neurons[i][j].nbConnections; k ++)
			{
				rdm = (double)rand() / (double)RAND_MAX * 2.0;
				if (rdm <= wError)
					lost.neurons[i][j].connectList[k].weight = 
						won.neurons[i][j].connectList[k].weight;
				else if (rdm > wError + lError)
					lost.neurons[i][j].connectList[k].weight += 
						(double)rand() / (double)RAND_MAX *
						(1 - lError) * 2 - (1 - lError);
			}
		}
	}
}

