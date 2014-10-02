#include "neuronsLearning.h"
#include "../Utils/math.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

Network NInitializeCompleteNetwork(ExempleSet exSet)
{
	Network r;
	r.nbLayers = 3;
	r.layersSize = (int*)malloc(sizeof(int) * 3);
	r.layersSize[0] = exSet.inputSize + 1;
	r.layersSize[1] = MPow(2, exSet.inputSize) + 1;
	r.layersSize[2] = exSet.targetSize;
	r.neurons = (Neuron**)malloc(sizeof(Neuron*) * r.nbLayers);
	srand(time(NULL));
	for (int i = 0; i < r.nbLayers; i ++)
		r.neurons[i] = (Neuron*)malloc(sizeof(Neuron) * r.layersSize[i]);
	for (int i = 0; i < r.nbLayers - 1; i ++)
	{
		for (int j = 0; j < r.layersSize[i]; j ++)
		{
			r.neurons[i][j].sum = 0;
			r.neurons[i][j].shock = 0;
			if (i < r.nbLayers - 1)
			{
				r.neurons[i][j].nbConnections = r.layersSize[i + 1];
				r.neurons[i][j].connectList = 
					(Connection*)malloc(sizeof(Connection)
					* r.layersSize[i + 1]);
				for (int k = 0; k < r.layersSize[i + 1]; k ++)
				{
					r.neurons[i][j].connectList[k].layer = i + 1;
					r.neurons[i][j].connectList[k].index = k;
					r.neurons[i][j].connectList[k].weight =
						(double)rand() / (double)RAND_MAX *
						(RAND_UP - RAND_DOWN) + RAND_DOWN;
				}
			}
			else
			{
				r.neurons[i][j].nbConnections = 0;
				r.neurons[i][j].connectList = NULL;
			}
		}
	}
	return r;
}

void NPrint(Network nWork)
{
	printf("nbLayers : %d\n\n", nWork.nbLayers);
	for (int i = 0 ; i < nWork.nbLayers; i ++)
	{
		printf("Layer : %d Size : %d\n", i, nWork.layersSize[i]);
		for (int j = 0; j < nWork.layersSize[i]; j ++)
		{
			printf("number : %d size : %d\n", j, 
				nWork.neurons[i][j].nbConnections);
			for (int k = 0; k < nWork.neurons[i][j].nbConnections; k++)
				printf("\t layer : %d index : %d weight : %f\n", 
					nWork.neurons[i][j].connectList[k].layer,
					nWork.neurons[i][j].connectList[k].index,
					nWork.neurons[i][j].connectList[k].weight);
		}
		printf("\n");
	}
}

int main()
{
	ExempleSet exSet;
	exSet.size = 4;
	exSet.inputSize = 2;
	exSet.targetSize = 1;
	exSet.exempleList = (Exemple*)malloc(sizeof(Exemple) * 4);
	for (int i = 0; i < exSet.size; i ++)
	{
		exSet.exempleList[i].input = (double*)malloc(sizeof(double)
			* exSet.inputSize);
		exSet.exempleList[i].target = (double*)malloc(sizeof(double)
			* exSet.targetSize);
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

	NPrint(NInitializeCompleteNetwork(exSet));
	return 0;
}
