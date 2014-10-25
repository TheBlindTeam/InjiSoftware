#include "learning.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int NDichotomicLearn(NetworkSet *nWorkSet)
{
	Network *nWon = nWorkSet->nWork;
	Network *nLost = nWon->sibling;
	double wError = nWon->error;
	double lError = nLost->error;
	if (wError < nWorkSet->maxError)
		return 0;
	NChangeLostNetwork(*nWon, wError, *nLost, lError);
	wError = NComputeError(nWon, nWorkSet->exSet, 0, NULL, 0);
	lError = NComputeError(nLost, nWorkSet->exSet, 0, NULL, 0);
	if (lError < wError)
		nWorkSet->nWork = nLost;
	return 1;
}

void NChangeLostNetwork(Network won, double wError, Network lost, double lError)
{
	double rdm;
	for (int i = 0; i < lost.nbLayers; i ++)
		for (int j = 0; j < lost.layersSize[i]; j ++)
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

int NBackPropLearn(NetworkSet *nWorkSet)
{
	double *output = NULL;
	if (nWorkSet->nWork->error < nWorkSet->maxError)
		return 0;
	Network *nWork = nWorkSet->nWork;
	Exemple *ex = nWorkSet->exSet.exemple;
	while(ex)
	{
		NRun(nWork, ex->input, &output);
		for (int j = 0; j < nWorkSet->exSet.targetSize; j ++)
		{
			Neuron *tmp = &nWork->neurons[nWork->nbLayers - 1][j];
			tmp->error =tmp->shockFoo.df(output[j])*(ex->target[j] - output[j]);
		}
		for (int j = nWork->nbLayers - 2; j > 0; j --)
			for (int k = 0; k < nWork->layersSize[j]; k++)
			{
				Neuron *tmp = &nWork->neurons[j][k];
				tmp->error = 0.0;
				for (int l = 0; l < tmp->nbConnections; l ++)
					tmp->error +=
						tmp->connectList[l].weight
						* nWork->neurons
						[tmp->connectList[l].layer]
						[tmp->connectList[l].index].error;
				tmp->error *= tmp->shockFoo.df(tmp->shock);
			}
		for (int j = 0; j < nWork->nbLayers; j ++)
			for (int k = 0; k < nWork->layersSize[j]; k++)
			{
				Neuron *tmp = &nWork->neurons[j][k];
				for (int l = 0; l < tmp->nbConnections; l++)
				{
					tmp->connectList[l].weight +=
						nWorkSet->momentum
						* tmp->connectList[l].prevChange;
					tmp->connectList[l].prevChange =
						nWorkSet->lRate
						* nWork->neurons
						[tmp->connectList[l].layer]
						[tmp->connectList[l].index].error
						* tmp->shock;
					tmp->connectList[l].weight +=
						tmp->connectList[l].prevChange;
				}
			}
		ex = ex->next;
	}
	NComputeError(nWork, nWorkSet->exSet, 0, NULL, 0);
	return 1;
}
