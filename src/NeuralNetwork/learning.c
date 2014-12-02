#include "learning.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int NDichotomicLearn(NetworkSet *nWorkSet)
{
	printf("DICHOTOMIC1\n");
	Network *nWon = nWorkSet->nWork;
	printf("DICHOTOMIC2\n");
	Network *nLost = nWon->sibling;
	printf("DICHOTOMIC3\n");
	double wError = nWon->error;
	double lError = nLost->error;
	printf("DICHOTOMIC4\n");
	if (wError < nWorkSet->maxError)
		return 0;
	printf("DICHOTOMIC5\n");
	NChangeLostNetwork(*nWon, wError, *nLost, lError);
	printf("DICHOTOMIC6\n");
	wError = NComputeError(nWon, nWorkSet->exSet, 0, NULL, 0);
	printf("DICHOTOMIC7\n");
	lError = NComputeError(nLost, nWorkSet->exSet, 0, NULL, 0);
	printf("DICHOTOMIC8\n");
	if (lError < wError)
		nWorkSet->nWork = nLost;
	printf("DICHOTOMIC9\n");
	return 1;
}

void NChangeLostNetwork(Network won, double wError, Network lost,
	double lError)
{
	double rdm;
	for (int i = 0; i < lost.nbLayers; i ++)
		for (int j = 0; j < lost.layersSize[i]; j ++)
			for (int k = 0; k < lost.neurons[i][j].nbConnections;
				k ++)
			{
				rdm = (double)rand() / (double)RAND_MAX * 2;
				if (rdm <= wError)
				{
					lost.neurons[i][j].connectList[k]
						.weight = won.neurons[i][j]
							.connectList[k].weight;
				}
				else if (rdm <= wError + lError)
				{
					lost.neurons[i][j].connectList[k]
						.weight += (double)rand() /
							(double)RAND_MAX *
							(1 - lError) * 2 -
							(1 - lError);
				}
			}
}

int NBackPropLearn(NetworkSet *nWorkSet)
{
	printf("BackProp1\n");
	if (nWorkSet->nWork->error < nWorkSet->maxError)
		return 0;
	printf("BackProp2\n");
	double *output = NULL;
	Network *nWork = nWorkSet->nWork;
	printf("BackProp3\n");
	printf("BackProp4\n");
		int index = rand() % nWorkSet->exSet->size;
	printf("BackProp4-1 %d\n", index);
		Exemple *ex = nWorkSet->exSet->exemple[index];
	printf("BackProp4-2\n");
		NRun(nWork, ex->input, &output);
	printf("BackProp5\n");
		for (int j = 0; j < nWorkSet->exSet->targetSize; j ++)
		{
			Neuron *tmp = &nWork->neurons[nWork->nbLayers - 1][j];
			tmp->error = FUNCTIONS[tmp->shockFoo].df(output[j]) *
				(ex->target[j] - output[j]);
		}
	printf("BackProp6\n");
		for (int j = nWork->nbLayers - 2; j > 0; j --)
			for (int k = 0; k < nWork->layersSize[j]; k++)
			{
				Neuron *tmp = &nWork->neurons[j][k];
				tmp->error = 0.0;
				for (int l = 0; l < tmp->nbConnections; l ++)
				{
					tmp->error += tmp->connectList[l].weight *
						nWork->neurons
						[tmp->connectList[l].layer]
						[tmp->connectList[l].index].error;
				}
				tmp->error *= FUNCTIONS[tmp->shockFoo].df(tmp->shock);
				for (int l = 0; l < tmp->nbConnections; l ++)
					tmp->error += pow(tmp->connectList[l].weight, 2) *
						nWorkSet->overfitCoef;
				}
	printf("BackProp7\n");
		for (int j = 0; j < nWork->nbLayers; j ++)
			for (int k = 0; k < nWork->layersSize[j]; k++)
			{
				Neuron *tmp = &nWork->neurons[j][k];
				for (int l = 0; l < tmp->nbConnections; l++)
				{
					tmp->connectList[l].weight +=
						nWorkSet->momentum
						* tmp->connectList[l]
						.prevChange;
					tmp->connectList[l].prevChange =
						nWorkSet->lRate
						* nWork->neurons
						[tmp->connectList[l].layer]
						[tmp->connectList[l].index]
						.error * tmp->shock;
					tmp->connectList[l].weight +=
						tmp->connectList[l].prevChange;
				}
			}
	printf("BackProp8\n");
	nWorkSet->lRate -= 0.05 * nWorkSet->lRate;
	printf("BackProp9\n");
	printf("BackProp10\n");
	NComputeError(nWork, nWorkSet->exSet, 0, NULL, 0);
	printf("BackProp11\n");
	return 1;
}
/*
int NBackPropLearn(NetworkSet *nWorkSet, int nb)
{
	double *output = NULL;
	if (nWorkSet->nWork->error < nWorkSet->maxError)
		return 0;
	Network *nWork = nWorkSet->nWork;
	for (int i = 0; i < nb; i ++)
	{
		int index = rand()
	Exemple *ex = nWorkSet->exSet.exemple[];
	{
		NRun(nWork, ex->input, &output);
		for (int j = 0; j < nWorkSet->exSet.targetSize; j ++)
		{
			Neuron *tmp = &nWork->neurons[nWork->nbLayers - 1][j];
			tmp->error = FUNCTIONS[tmp->shockFoo].df(output[j])*(ex->target[j]
					- output[j]);
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
						[tmp->connectList[l].index]
						.error;
				tmp->error *= FUNCTIONS[tmp->shockFoo].df(tmp->shock);
			}
		ex = ex->next;
	}
	return 1;
}*/
