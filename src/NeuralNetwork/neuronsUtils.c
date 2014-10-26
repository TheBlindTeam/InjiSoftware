#include "neuronsUtils.h"
#include "learning.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const double RAND_UP = 5;
const double RAND_DOWN = -5;
const Ninit NINIT[] = {
	&NInitializeCompleteBias,
	&NInitializeCompleteNBias,
	&NInitializeLinearBias,
	&NInitializeLinearNBias };


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
				r->neurons[i][j].nbConnections = r->layersSize[i + 1]
					- ((r->bias && i != r->nbLayers - 2) ? 1 : 0);
				r->neurons[i][j].connectList =
					malloc(sizeof(Connection) * r->neurons[i][j].nbConnections);
				for (int k = 0; k < r->neurons[i][j].nbConnections; k ++)
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

Network *NInitializeLinearBias(int input, int output)
{
	return NInitializeSimpleMLP(input, output, input, 1);
}

Network *NInitializeLinearNBias(int input, int output)
{
	return NInitializeSimpleMLP(input, output, input, 0);
}

void NInitThresHoldSimpleMLP(Network *nWork, FAndDifF input, FAndDifF output,
	FAndDifF bias, FAndDifF others)
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

void NInitEdge(Network *nWork, int startL, int startI, int endL, int endI,int k)
{
	nWork->neurons[startL][startI].connectList[k].layer = endL;
	nWork->neurons[startL][startI].connectList[k].index = endI;
	nWork->neurons[startL][startI].connectList[k].prevChange = 0.0;
	nWork->neurons[startL][startI].connectList[k].weight =
						(double)rand() / (double)RAND_MAX *
						(RAND_UP - RAND_DOWN) + RAND_DOWN;
}

void NInitializeSumNetwork(Network *nWork)
{
	for (int i = 0; i < nWork->nbLayers; i ++)
		for (int j = 0; j < nWork->layersSize[i]; j ++)
			if(nWork->bias && nWork->layersSize[i]-1==j && nWork->nbLayers-1!=i)
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
			if (i == 0 && (!nWork->bias || j < nWork->layersSize[i] - 1))
				tmp->sum = input[j];
			tmp->shock = tmp->shockFoo.f(tmp->sum);
			for (int k = 0; k < tmp->nbConnections; k ++)
			{
				nWork->neurons
					[tmp->connectList[k].layer]
					[tmp->connectList[k].index].sum +=
					tmp->connectList[k].weight * tmp->shock;
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

double NComputeError(Network *nWork, ExempleSet exSet,
	int print, char *r, int max)
{
	int count = 0;
	double totalError = 0;
	double error;
	double *output = NULL;
	char tmp[10];
	Exemple *ex = exSet.exemple;
	int i = 0;
	if (print)
		r[0] = 0;
	while (ex)
	{
		NRun(nWork, ex->input, &output);
		error = USquarredError(ex->target, output, exSet.targetSize);
		totalError += error;
		if(print)
		{
			addInStringAt(r, "error: ", &i, max);
			snprintf(tmp, 9, "%lf", error);
			addInStringAt(r, tmp, &i, max);
			addInStringAt(r, "    input -> ", &i, max);
			for (int j = 0; j < exSet.inputSize; j ++)
			{
				snprintf(tmp, 9, "%lf", ex->input[j]);
				addInStringAt(r, tmp, &i, max);
				addInStringAt(r, " ", &i, max);
			}
			addInStringAt(r, "    output -> ", &i, max);
			for (int j = 0; j < exSet.targetSize; j ++)
			{
				snprintf(tmp, 9, "%lf", output[j]);
				addInStringAt(r, tmp, &i, max);
				addInStringAt(r, " ", &i, max);
			}
			addInStringAt(r, "    target -> ", &i, max);
			for (int j = 0; j < exSet.targetSize; j ++)
			{
				snprintf(tmp, 9, "%lf", ex->target[j]);
				addInStringAt(r, tmp, &i, max);
				addInStringAt(r, " ", &i, max);
			}
			addInStringAt(r, "\n", &i, max);
		}
		ex = ex->next;
		count++;
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
			for (int k = 0; k < nWork.neurons[i][j].nbConnections; k++)
				printf("\t\t\tlayer: %d index: %d weight: %f\n",
					nWork.neurons[i][j].connectList[k].layer,
					nWork.neurons[i][j].connectList[k].index,
					nWork.neurons[i][j].connectList[k].weight);
		}
		printf("\n");
	}
}

void addInExempleSet(ExempleSet *exSet, double *input, int inputSize,
	double *target, int targetSize)
{
	Exemple *ex = malloc(sizeof(Exemple));
	ex->next = NULL;
	ex->input = malloc(sizeof(double) * inputSize);
	ex->target = malloc(sizeof(double) * targetSize);
	for (int i = 0; i < inputSize; i ++)
		ex->input[i] = input[i];
	for (int i = 0; i < targetSize; i ++)
		ex->target[i] = target[i];
	if (!exSet->exemple)
		exSet->exemple = ex;
	else
	{
		Exemple *tmp = exSet->exemple;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = ex;
	}
}

ExempleSet NGetExempleSet(double *input[], int inputDim2,
	double *target[], int targetDim2, int size)
{
	ExempleSet r;
	r.inputSize = inputDim2;
	r.targetSize = targetDim2;
	r.exemple = NULL;
	for (int i = 0; i < size; i ++)
		addInExempleSet(&r, input[i], inputDim2, target[i], targetDim2);
	return r;
}

ExempleSet NGetAndExempleSet()
{
	double **input = malloc(sizeof(double) * 4);
	double **target = malloc(sizeof(double) * 4);
	for (int i = 0; i < 4; i ++)
	{
		input[i] = malloc(sizeof(double) * 2);
		target[i] = malloc (sizeof(double));
		input[i][0] = i % 2;
		input[i][1] = i / 2;
		target[i][0] = i == 3 ? 1 : 0;
	}
	ExempleSet r = NGetExempleSet(input, 2, target, 1, 4);
	for (int i = 0; i < 4; i ++)
	{
		free(input[i]);
		free(target[i]);
	}
	free(input);
	free(target);
	return r;
}

ExempleSet NGetOrExempleSet()
{
	double **input = malloc(sizeof(double) * 4);
	double **target = malloc(sizeof(double) * 4);
	for (int i = 0; i < 4; i ++)
	{
		input[i] = malloc(sizeof(double) * 2);
		target[i] = malloc (sizeof(double));
		input[i][0] = i % 2;
		input[i][1] = i / 2;
		target[i][0] = i > 1 ? 1 : 0;
	}
	ExempleSet r = NGetExempleSet(input, 2, target, 1, 4);
	for (int i = 0; i < 4; i ++)
	{
		free(input[i]);
		free(target[i]);
	}
	free(input);
	free(target);
	return r;
}

ExempleSet NGetXorExempleSet()
{
	double **input = malloc(sizeof(double) * 4);
	double **target = malloc(sizeof(double) * 4);
	for (int i = 0; i < 4; i ++)
	{
		input[i] = malloc(sizeof(double) * 2);
		target[i] = malloc (sizeof(double));
		input[i][0] = i % 2;
		input[i][1] = i / 2;
		target[i][0] = i == 1 || i == 2 ? 1 : 0;
	}
	ExempleSet r = NGetExempleSet(input, 2, target, 1, 4);
	for (int i = 0; i < 4; i ++)
	{
		free(input[i]);
		free(target[i]);
	}
	free(input);
	free(target);
	return r;
}

NetworkSet* NInitNetworkSet(int gate, int archi, int learning, int input,
	int output, int others, int bias)
{
	NetworkSet* r = malloc(sizeof(NetworkSet));

	r->maxError = 0.000001;
	r->lRate = 0.06;
	r->momentum = 0.2;
	if (gate == 0)
		r->exSet = NGetAndExempleSet();
	else if (gate == 1)
		r->exSet = NGetOrExempleSet();
	else
		r->exSet = NGetXorExempleSet();
	if (learning == 0)
	{
		Network *N1 = NINIT[archi](r->exSet.inputSize, r->exSet.targetSize);
		Network *N2 = NINIT[archi](r->exSet.inputSize, r->exSet.targetSize);

		NInitThresHoldSimpleMLP(N1, FUNCTIONS[input], FUNCTIONS[output],
			FUNCTIONS[bias], FUNCTIONS[others]);
		NInitThresHoldSimpleMLP(N2, FUNCTIONS[input], FUNCTIONS[output],
			FUNCTIONS[bias], FUNCTIONS[others]);
		N1->sibling = N2;
		N2->sibling = N1;
		NComputeError(N1, r->exSet, 0, NULL, 0);
		NComputeError(N2, r->exSet, 0, NULL, 0);
		if (N1->error < N2->error)
			r->nWork = N1;
		else
			r->nWork = N2;
		r->learn = &NDichotomicLearn;
	}
	else
	{
			r->nWork = NINIT[archi](r->exSet.inputSize, r->exSet.targetSize);
			NInitThresHoldSimpleMLP(r->nWork, FUNCTIONS[input]
				, FUNCTIONS[output], FUNCTIONS[bias], FUNCTIONS[others]);
			NComputeError(r->nWork, r->exSet, 0, NULL, 0);
			r->learn = &NBackPropLearn;
	}
	return r;
}

NetworkSet* NDefaultNetworkSet()
{
	NetworkSet* r = malloc(sizeof(NetworkSet));
	r->learn = &NBackPropLearn;
	r->nWork = NInitializeCompleteBias(2, 1);
	r->maxError = 0.000001;
	r->lRate = 0.06;
	r->momentum = 0.2;
	r->exSet = NGetXorExempleSet();
	NInitThresHoldSimpleMLP(r->nWork, LINEAR, LINEAR, TAN_SIGMOID, TAN_SIGMOID);
	NComputeError(r->nWork, r->exSet, 0, NULL, 0);
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
	if(exemple)
	{
		NFreeExemple(exemple->next);
		free(exemple->input);
		free(exemple->target);
		free(exemple);
	}
}

void NFreeExempleSet(ExempleSet exSet)
{
	NFreeExemple(exSet.exemple);
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
