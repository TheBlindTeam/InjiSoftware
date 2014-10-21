#include "neuronsUtils.h"
#include "learning.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

const double RAND_UP = 5;
const double RAND_DOWN = -5;

Network *NInitializeSimpleMLP(int input, int output, int middle, int bias)
{
	Network *r = malloc(sizeof(struct Network));
	r->nbLayers = 3;
	r->bias = bias ? 1 : 0;
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

double NComputeError(Network *nWork, ExempleSet exSet, int (*print)(char*))
{
	int count = 0;
	double totalError = 0;
	char tmp[10];
	double error;
	double *output = NULL;
	Exemple *ex = exSet.exemple;
	while (ex)
	{
		NRun(nWork, ex->input, &output);
		error = USquarredError(ex->target, output, exSet.targetSize);
		totalError += error;
		if(print)
		{
			print("\terror: ");
			snprintf(tmp, 10, "%f", error);
			print(tmp);
			print("\tinput -> ");
			for (int j = 0; j < exSet.inputSize; j ++)
			{
				snprintf(tmp, 10, "%f", ex->input[j]);
				print(tmp);
				print(" ");
			}
			print("output -> ");
			for (int j = 0; j < exSet.targetSize; j ++)
			{
				snprintf(tmp, 10, "%f", output[j]);
				print(tmp);
				print(" ");
			}
			print("target -> ");
			for (int j = 0; j < exSet.targetSize; j ++)
			{
				snprintf(tmp, 10, "%f", ex->target[j]);
				print(tmp);
				print(" ");
			}
			print("\n");
		}
		ex = ex->next;
		count++;
	}
	totalError /= count;
	if (print)
	{
		print("average error: ");
		snprintf(tmp, 10, "%f", totalError);
		print(tmp);
		print("\n");
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

ExempleSet NGetXorExempleSet()
{
	double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double target[4][1] = {{0}, {1}, {1}, {0}};
	ExempleSet exSet;
	exSet.inputSize = 2;
	exSet.targetSize = 1;
	exSet.exemple = NULL;
	for (int i = 0; i < 4; i ++)
		addInExempleSet(&exSet, input[i], 2, target[i], 1);
	return exSet;
}

NetworkSet NInitNetworkSet(int *arg);

NetworkSet NDefaultNetworkSet()
{
	NetworkSet r;
	r.learn = &NBackPropLearn;
	r.nWork = NInitializeLinearBias(2, 1);
	r.maxError = 0.000001;
	r.lRate = 0.06;
	r.momentum = 0.2;
	r.exSet = NGetXorExempleSet();
	NInitThresHoldSimpleMLP(r.nWork, LINEAR, LINEAR, TAN_SIGMOID, TAN_SIGMOID);
	NComputeError(r.nWork, r.exSet, NULL);
	return r;
}

int specialPrint(char *s)
{
	printf("%s", s);
	return 1;
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

/*int main()
{
	srand(time(NULL));
	for(;;)
	{
		int count = 0;
		NetworkSet nWorkSet = NDefaultNetworkSet();
		while(nWorkSet.learn(&nWorkSet))
		{
			count ++;
			if (count > 10000)
				break;
		}
		NPrintNetwork(*nWorkSet.nWork);
		NComputeError(nWorkSet.nWork, nWorkSet.exSet, specialPrint);
		printf("iterations: %d\n", count);
		getchar();
	}
	return 0;
}*/

