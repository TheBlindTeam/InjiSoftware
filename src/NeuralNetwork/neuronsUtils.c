#include "neuronsUtils.h"
#include "learning.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const double RAND_UP = 1;
const double RAND_DOWN = -1;
const Ninit NINIT[] = {
	&NInitializeCompleteBias,
	&NInitializeCompleteNBias,
	&NInitializeLinearBias,
	&NInitializeLinearNBias,
	&NInitializeDoubleBias,
	&NInitializeDoubleNBias};


Network *NInitializeSimpleMLP(int input, int output, int middle, int bias)
{
	Network *r = malloc(sizeof(struct Network));
	r->nbLayers = 3;
	r->bias = bias ? 1 : 0;
	r->sibling = NULL;
	r->layersSize = malloc(sizeof(int) * 3);
	r->layersSize[0] = input + r->bias;
	r->layersSize[1] = middle + r->bias;
	r->layersSize[2] = output;
	r->neurons = malloc(sizeof(Neuron*) * r->nbLayers);
	for (int i = 0; i < r->nbLayers; i ++)
		r->neurons[i] = malloc(sizeof(Neuron) * r->layersSize[i]);
	for (int i = 0; i < r->nbLayers; i ++)
		for (int j = 0; j < r->layersSize[i]; j ++)
			if (i < r->nbLayers - 1)
			{
				r->neurons[i][j].nbConnections =
					r->layersSize[i + 1]
					- ((r->bias && i != r->nbLayers - 2) ?
						1 : 0);
				r->neurons[i][j].connectList =
					malloc(sizeof(Connection) *
					r->neurons[i][j].nbConnections);
				for (int k = 0;
					k < r->neurons[i][j].nbConnections;
						k++)
					NInitEdge(r, i, j, i + 1, k, k);
			}
			else
			{
				r->neurons[i][j].nbConnections = 0;
				r->neurons[i][j].connectList = NULL;
			}
	return r;
}

Network *NInitializeCompleteBias(int input, int output)
{
	return NInitializeSimpleMLP(input, output, UPow(2, input), 1);
}

Network *NInitializeCompleteNBias(int input, int output)
{ 
	return NInitializeSimpleMLP(input, output, UPow(2, input), 0);
}

Network *NInitializeDoubleBias(int input, int output)
{
	return NInitializeSimpleMLP(input, output, 2 * input, 1);
}

Network *NInitializeDoubleNBias(int input, int output)
{
	return NInitializeSimpleMLP(input, output, 2 * input, 0);
}

Network *NInitializeLinearBias(int input, int output)
{
	return NInitializeSimpleMLP(input, output, input, 1);
}

Network *NInitializeLinearNBias(int input, int output)
{
	return NInitializeSimpleMLP(input, output, input, 0);
}

void NInitThresHoldSimpleMLP(Network *nWork, FunctionId input,FunctionId output,
	FunctionId bias, FunctionId others)
{
	for (int i = 0; i < nWork->nbLayers; i ++)
		for (int j = 0; j < nWork->layersSize[i]; j ++)
		{
			nWork->neurons[i][j].shockFoo = others;
			if (i == nWork->nbLayers - 1)
				nWork->neurons[i][j].shockFoo = output;
			else if (j == nWork->layersSize[i] - 1 && nWork->bias)
				nWork->neurons[i][j].shockFoo = bias;
			else if (i == 0)
				nWork->neurons[i][j].shockFoo = input;
		}
}

void NInitEdge(Network *nWork, int startL, int startI, int endL, int endI,
	int k)
{
	nWork->neurons[startL][startI].connectList[k].layer = endL;
	nWork->neurons[startL][startI].connectList[k].index = endI;
	nWork->neurons[startL][startI].connectList[k].prevChange = 0.0;
	nWork->neurons[startL][startI].connectList[k]
		.weight = (double)rand() / (double)RAND_MAX *
			(RAND_UP - RAND_DOWN) + RAND_DOWN;
}

void NInitializeSumNetwork(Network *nWork)
{
	for (int i = 0; i < nWork->nbLayers; i ++)
		for (int j = 0; j < nWork->layersSize[i]; j ++)
			if(nWork->bias && nWork->layersSize[i]-1==j
					&& nWork->nbLayers-1!=i)
				nWork->neurons[i][j].sum = 1;
			else
				nWork->neurons[i][j].sum = 0;
}

int NRun(Network *nWork, double *input, double **r)
{
	int rSize;

	NInitializeSumNetwork(nWork);
	if (nWork->nbLayers <= 0)
		return 0;
	for (int i = 0; i < nWork->nbLayers; i ++)
		for (int j = 0; j < nWork->layersSize[i] ; j ++)
		{
			Neuron *tmp = &nWork->neurons[i][j];
			if (i == 0 && (!nWork->bias ||
					j < nWork->layersSize[i] - 1))
				tmp->sum = input[j];
			tmp->shock = FUNCTIONS[tmp->shockFoo].f(tmp->sum);
			for (int k = 0; k < tmp->nbConnections; k ++)
			{
				nWork->neurons
					[tmp->connectList[k].layer]
					[tmp->connectList[k].index].sum +=
					tmp->connectList[k].weight *
						tmp->shock;
			}
		}
	rSize = nWork->layersSize[nWork->nbLayers - 1];
	*r = malloc(sizeof(double) * rSize);
	for (int i = 0; i < rSize; i ++)
		(*r)[i] = nWork->neurons[nWork->nbLayers - 1][i].shock;
	return 1;
}

void addInStringAt(char *r, char *s2, int *at, int max)
{
	for (int i = 0; s2[i] && *at < max; i ++)
	{
		r[*at] = s2[i];
		(*at)++;
	}
	r[*at] = 0;
}

double NComputeError(Network *nWork, ExempleSet *exSet,
	int print, char *r, int max)
{
	int count = 0;
	double totalError = 0;
	double error;
	double *output = NULL;
	char tmp[10];
	int i = 0;
	if (print)
		r[0] = 0;
	for (int index = 0; index < exSet->size; index ++)
	{
		Exemple *ex = exSet->exemple[index];
		NRun(nWork, ex->input, &output);
		error = USquarredError(ex->target, output, exSet->targetSize);
		totalError += error;
		if(print)
		{
			addInStringAt(r, "error: ", &i, max);
			snprintf(tmp, 9, "%lf", error);
			addInStringAt(r, tmp, &i, max);
			addInStringAt(r, "    input -> ", &i, max);
			for (int j = 0; j < exSet->inputSize; j ++)
			{
				snprintf(tmp, 9, "%lf", ex->input[j]);
				addInStringAt(r, tmp, &i, max);
				addInStringAt(r, " ", &i, max);
			}
			addInStringAt(r, "    output -> ", &i, max);
			for (int j = 0; j < exSet->targetSize; j ++)
			{
				snprintf(tmp, 9, "%lf", output[j]);
				addInStringAt(r, tmp, &i, max);
				addInStringAt(r, " ", &i, max);
			}
			addInStringAt(r, "    target -> ", &i, max);
			for (int j = 0; j < exSet->targetSize; j ++)
			{
				snprintf(tmp, 9, "%lf", ex->target[j]);
				addInStringAt(r, tmp, &i, max);
				addInStringAt(r, " ", &i, max);
			}
			addInStringAt(r, "\n", &i, max);
		}
		count++;
		free(output);
	}
	totalError /= count;
	if (print)
	{
		addInStringAt(r, "average error: ", &i, max);
		snprintf(tmp, 9, "%f", totalError);
		addInStringAt(r, tmp, &i, max);
		addInStringAt(r, "\n", &i, max);
	}
	nWork->error = totalError;
	return totalError;
}

void NPrintNetwork(Network nWork)
{
	if (nWork.nbLayers <= 1)
		printf("There is %d layer.\n", nWork.nbLayers);
	else
		printf("There are %d layers.\n", nWork.nbLayers);
	for (int i = 0 ; i < nWork.nbLayers; i ++)
	{
		printf("\tLayer: %d Size:  %d\n", i, nWork.layersSize[i]);
		for (int j = 0; j < nWork.layersSize[i]; j ++)
		{
			printf("\t\tnumber: %d size: %d\n",
				j, nWork.neurons[i][j].nbConnections);
			for (int k = 0; k < nWork.neurons[i][j].nbConnections;
					k++)
				printf("\t\t\tlayer: %d index: %d weight: %f\n",
					nWork.neurons[i][j].connectList[k]
						.layer,
					nWork.neurons[i][j].connectList[k]
						.index,
					nWork.neurons[i][j].connectList[k]
						.weight);
		}
		printf("\n");
	}
}

void addInExempleSet(ExempleSet *exSet, double *input, double *target)
{
	if (exSet->size == exSet->capacity)
	{
		exSet->exemple = realloc(exSet->exemple, 2 * exSet->capacity * sizeof(Exemple));
		exSet->capacity *= 2;
	}
	Exemple *ex = malloc(sizeof(Exemple));
	ex->input = malloc(sizeof(double) * exSet->inputSize);
	ex->target = malloc(sizeof(double) * exSet->targetSize);
	for (int i = 0; i < exSet->inputSize; i ++)
		ex->input[i] = input[i];
	for (int i = 0; i < exSet->targetSize; i ++)
		ex->target[i] = target[i];
	exSet->exemple[exSet->size] = ex;
	exSet->size++;
}

ExempleSet *NGetExempleSet(double *input[], int inputDim2,
	double *target[], int targetDim2, int size)
{
	ExempleSet *r = malloc(sizeof(ExempleSet));
	r->inputSize = inputDim2;
	r->targetSize = targetDim2;
	r->exemple = malloc(sizeof(Exemple) * size);
	r->size = 0;
	r->capacity = size;
	for (int i = 0; i < size; i ++)
		addInExempleSet(r, input[i], target[i]);
	return r;
}

void NFreeNeuron(Neuron *neuron)
{
	free(neuron->connectList);
}

void NFreeNetwork(Network *nWork)
{
	for (int i = 0; i < nWork->nbLayers; i ++)
		free(nWork->neurons[i]);
	free(nWork->neurons);
	free(nWork->layersSize);
	free(nWork);
}

void NFreeExemple(Exemple *exemple)
{
		free(exemple->input);
		free(exemple->target);
		free(exemple);
}

void NFreeExempleSet(ExempleSet *exSet)
{
	for (int i = 0; i < exSet->size; i ++)
		NFreeExemple(exSet->exemple[i]);
	free(exSet->exemple);
}

void NFreeNetworkSet(NetworkSet* nWorkset)
{
	if(nWorkset->nWork->sibling)
		NFreeNetwork(nWorkset->nWork->sibling);
	NFreeNetwork(nWorkset->nWork);
	NFreeExempleSet(nWorkset->exSet);
	free(nWorkset);
}

int getMaxNeuronsLayer(Network nWork)
{
	int max = 0;
	for (int l = 0; l < nWork.nbLayers; l ++)
		if (nWork.layersSize[l] > max)
			max = nWork.layersSize[l];
	return max;
}

int getTotalNeuronsAmount(Network nWork)
{
	int count = 0;
	for (int l = 0; l < nWork.nbLayers; l ++)
		count += nWork.layersSize[l];
	return count;
}
