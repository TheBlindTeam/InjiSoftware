#include "neuronsUtils.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Utils/math.h"
const double RAND_UP = 10;
const double RAND_DOWN = -10;

Network NInitializeCompleteNetwork(ExempleSet exSet, FAndDifF shockFoo)
{
	Network r;
	r.nbLayers = 3;
	r.bias = 1;
	r.layersSize = malloc(sizeof(int) * 3);
	r.layersSize[0] = exSet.inputSize + 1;
	r.layersSize[1] = UPow(2, exSet.inputSize) + 1;
	r.layersSize[2] = exSet.targetSize + 1;
	r.neurons = malloc(sizeof(Neuron*) * r.nbLayers);
	for (int i = 0; i < r.nbLayers; i ++)
		r.neurons[i] = malloc(sizeof(Neuron) * r.layersSize[i]);
	for (int i = 0; i < r.nbLayers; i ++)
		for (int j = 0; j < r.layersSize[i]; j ++)
		{
			if (i == 0 || j == r.layersSize[i] - 1)
				r.neurons[i][j].shockFoo = LINEAR;
			else
				r.neurons[i][j].shockFoo = shockFoo;
			if (i < r.nbLayers - 1)
			{
				r.neurons[i][j].nbConnections = r.layersSize[i + 1] - 1;
				r.neurons[i][j].connectList =
					malloc(sizeof(Connection) * r.neurons[i][j].nbConnections);
				for (int k = 0; k < r.neurons[i][j].nbConnections; k ++)
					NInitEdge(r, i, j, i + 1, k, k);
			}
			else
			{
				r.neurons[i][j].nbConnections = 0;
				r.neurons[i][j].connectList = NULL;
			}
		}
	return r;
}

void NInitEdge(Network nWork, int startL, int startI, int endL, int endI, int k)
{
	nWork.neurons[startL][startI].connectList[k].layer = endL;
	nWork.neurons[startL][startI].connectList[k].index = endI;
	nWork.neurons[startL][startI].connectList[k].prevChange = 0.0;
	nWork.neurons[startL][startI].connectList[k].weight =
						(double)rand() / (double)RAND_MAX *
						(RAND_UP - RAND_DOWN) + RAND_DOWN;
}

void NInitializeSumNetwork(Network nWork)
{
	for (int i = 0; i < nWork.nbLayers; i ++)
		for (int j = 0; j < nWork.layersSize[i]; j ++)
			if (j == nWork.layersSize[i] - 1 && nWork.bias)
				nWork.neurons[i][j].sum = 1;
			else
				nWork.neurons[i][j].sum = 0;
}

double *NRun(Network nWork, double *input)
{
	double *r;
	int rSize;

	NInitializeSumNetwork(nWork);
	if (nWork.nbLayers <= 0)
		return NULL;
	for (int i = 0; i < nWork.nbLayers; i ++)
		for (int j = 0; j < nWork.layersSize[i] ; j ++)
		{
			Neuron *tmp = &nWork.neurons[i][j];
			if (i == 0)
				tmp->sum = input[j];
			tmp->shock = tmp->shockFoo.f(tmp->sum);
			for (int k = 0; k < tmp->nbConnections; k ++)
			{
				nWork.neurons
					[tmp->connectList[k].layer]
					[tmp->connectList[k].index].sum +=
						tmp->connectList[k].weight * tmp->shock;
			}
		}
	rSize = nWork.layersSize[nWork.nbLayers - 1];
	r = malloc(sizeof(double) * rSize);
	for (int i = 0; i < rSize; i ++)
		r[i] = nWork.neurons[nWork.nbLayers - 1][i].shock;
	return r;
}

double NComputeSquarredError(Network nWork, ExempleSet exSet, int needPrint)
{
	double totalError = 0;
	double error;
	double *output;
	for (int i = 0; i < exSet.size; i++)
	{
		output = NRun(nWork, exSet.exempleList[i].input);
		error = USquarredError(output, exSet.exempleList[i].target,
					exSet.targetSize);
		totalError += error;
		if(needPrint)
		{
			printf("error : %f\n\t", error);
			for (int j = 0; j < exSet.inputSize; j ++)
				printf("input_%d : %f ", j, exSet.exempleList[i].input[j]);
			printf("\n\t");
			for (int j = 0; j < exSet.targetSize; j++)
				printf("output_%d : %f ", j, output[j]);
			printf("\n\t");
			for (int j = 0; j < exSet.targetSize; j++)
				printf("target_%d : %f ", j, exSet.exempleList[i].target[j]);
			printf("\n");
		}
	}
	totalError /= exSet.size;
	if (needPrint)
		printf("totalError : %f\n", totalError);
	return totalError;
}

void NPrintNetwork(Network nWork)
{
	printf("nbLayers : %d\n\n", nWork.nbLayers);
	for (int i = 0 ; i < nWork.nbLayers; i ++)
	{
		printf("Layer : %d Size : %d\n", i, nWork.layersSize[i]);
		for (int j = 0; j < nWork.layersSize[i]; j ++)
		{
			printf("number : %d size : %d error : %f sum : %f shock : %f\n",
				j, nWork.neurons[i][j].nbConnections, nWork.neurons[i][j].error,
				nWork.neurons[i][j].sum, nWork.neurons[i][j].shock);
			for (int k = 0; k < nWork.neurons[i][j].nbConnections; k++)
				printf("\tlayer : %d index : %d weight : %f prevChange : %f\n", 
					nWork.neurons[i][j].connectList[k].layer,
					nWork.neurons[i][j].connectList[k].index,
					nWork.neurons[i][j].connectList[k].weight,
					nWork.neurons[i][j].connectList[k].prevChange);
		}
		printf("\n");
	}
}

int main()
{
	ExempleSet exSet;
	Network nWork;

	srand(time(NULL));
	exSet.size = 4;
	exSet.inputSize = 2;
	exSet.targetSize = 1;
	exSet.exempleList = malloc(sizeof(Exemple) * 4);
	for (int i = 0; i < exSet.size; i ++)
	{
		exSet.exempleList[i].input = malloc(sizeof(double) * exSet.inputSize);
		exSet.exempleList[i].target = malloc(sizeof(double) * exSet.targetSize);
	}
	exSet.exempleList[0].input[0] = 0;
	exSet.exempleList[0].input[1] = 0;
	exSet.exempleList[0].target[0] = 0;
	exSet.exempleList[1].input[0] = 0;
	exSet.exempleList[1].input[1] = 1;
	exSet.exempleList[1].target[0] = 1;
	exSet.exempleList[2].input[0] = 1;
	exSet.exempleList[2].input[1] = 0;
	exSet.exempleList[2].target[0] = 1;
	exSet.exempleList[3].input[0] = 1;
	exSet.exempleList[3].input[1] = 1;
	exSet.exempleList[3].target[0] = 0;

	nWork = NGetDichotomicTrainingNetwork(exSet, 0.00001);
	NPrintNetwork(nWork);
	NComputeSquarredError(nWork, exSet, 1);
	nWork = NGetBackPropTrainingNetwork(exSet, 0.00001, 0.005, 0.008);
	NPrintNetwork(nWork);
	NComputeSquarredError(nWork, exSet, 1);
	return 0;
}

Network NGetBackPropTrainingNetwork(ExempleSet exSet, double maxError, double momentum, double lRate)
{
	Network nWork;
	double *output;
	nWork = NInitializeCompleteNetwork(exSet, SIGMOID);
	int round = 0;
	while (NComputeSquarredError(nWork, exSet, 0) > maxError)
	{
		for (int i = 0; i < exSet.size; i++)
		{
			output = NRun(nWork, exSet.exempleList[i].input);
			for (int j = 0; j < exSet.targetSize; j ++)
			{
				Neuron *tmp = &nWork.neurons[nWork.nbLayers - 1][j];
				tmp->error = tmp->shockFoo.df(output[j]) * (exSet.exempleList[i].target[j] - output[j]);
			}
			for (int j = nWork.nbLayers - 2; j > 0; j --)
				for (int k = 0; k < nWork.layersSize[j]; k++)
				{
					Neuron *tmp = &nWork.neurons[j][k];
					tmp->error = 0.0;
					for (int l = 0; l < tmp->nbConnections; l ++)
						tmp->error += tmp->connectList[l].weight * nWork.neurons
																	[tmp->connectList[l].layer]
																	[tmp->connectList[l].index].error;
					tmp->error *= tmp->shockFoo.df(tmp->shock);
				}
			for (int j = 0; j < nWork.nbLayers; j ++)
				for (int k = 0; k < nWork.layersSize[j]; k++)
				{
					Neuron *tmp = &nWork.neurons[j][k];
					for (int l = 0; l < tmp->nbConnections; l++)
					{
						tmp->connectList[l].weight += momentum * tmp->connectList[l].prevChange;
						tmp->connectList[l].prevChange =  lRate * nWork.neurons[tmp->connectList[l].layer]
																				[tmp->connectList[l].index].error * tmp->shock;
						tmp->connectList[l].weight += tmp->connectList[l].prevChange;
					}
				}
		}
		printf("%d  %f haaaaaaaaaaaaaaaaaa\n", round++, NComputeSquarredError(nWork, exSet, 0));
	}
	return nWork;
}

Network NGetDichotomicTrainingNetwork(ExempleSet exSet, double maxError)
{
	Network nWorks[2];
	double errors[2];
	srand(time(NULL));
	int round = 0;
	for (int i = 0; i < 2; i ++)
	{
		nWorks[i] = NInitializeCompleteNetwork(exSet, SIGMOID);
		errors[i] = NComputeSquarredError(nWorks[i], exSet, 0);
	}
	do
	{
		if (errors[0] > errors[1])
			NChangeLostNetwork(nWorks[1], errors[1], nWorks[0], errors[0]);
		else
			NChangeLostNetwork(nWorks[0], errors[0], nWorks[1], errors[1]);
	 	for (int i = 0; i < 2; i ++)
			errors[i] = NComputeSquarredError(nWorks[i], exSet, 0);
		round ++;
	}while(errors[0] > maxError && errors[1] > maxError);
	printf("round : %d\n", round);
	if (errors[0] < maxError)
		return nWorks[0];
	return nWorks[1];
}

void NChangeLostNetwork(Network won, double wError, Network lost, double lError)
{
	double rdm;
	for (int i = 0; i < lost.nbLayers; i ++)
		for (int j = 0; j < lost.layersSize[i]; j ++)
			for (int k = 0; k < lost.neurons[i][j].nbConnections; k ++)
			{
				rdm = (double)rand() / (double)RAND_MAX * 2.0;
				if (rdm <= 1 - wError)
					lost.neurons[i][j].connectList[k].weight = 
						won.neurons[i][j].connectList[k].weight;
				else if (rdm > 2 - (wError + lError))
					lost.neurons[i][j].connectList[k].weight += 
						(double)rand() / (double)RAND_MAX *
						(1 - lError) * 2 - (1 - lError);
			}
}

