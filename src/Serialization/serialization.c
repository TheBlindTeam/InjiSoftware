#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "serialization.h"
#include "../Utils/math.h"

int writeConnection(Connection ct, FILE *file)
{
	fprintf(file, "\t\t\t\tLAY: %d\n", ct.layer);
	fprintf(file, "\t\t\t\tIND: %d\n", ct.index);
	fprintf(file, "\t\t\t\tWEI: ");
	if (ct.weight < 0)
	{
		fprintf(file, "-");
		ct.weight *= -1;
	}
	fprintf(file, "%d.%d\n", (int)ct.weight,
	(int)((ct.weight - (int)(ct.weight))*1000000));
	//fprintf(file, "\t\t\t\tWEI: %.6lf\n", ct.weight);
	return 1;
}

int writeNeuron(Neuron neuron, FILE *file)
{
	fprintf(file, "\t\t\tFOO: %d\n", neuron.shockFoo);
	fprintf(file, "\t\t\tNBC: %d\n", neuron.nbConnections);
	for (int i = 0; i < neuron.nbConnections; i++)
	{
			fprintf(file, "\t\t\tCO%d:\n", i);
			writeConnection(neuron.connectList[i], file);
	}
	return 1;
}

int writeNetwork(Network n, FILE *file)
{
	fprintf(file, "NET:\n");
	fprintf(file, "\tNBL: %d\n", n.nbLayers);
	for (int i = 0; i < n.nbLayers; i++)
		fprintf(file, "\t\tLA%d: %d\n", i, n.layersSize[i]);
	fprintf(file, "\tBIA: %d\n", n.bias);
	for (int i = 0; i < n.nbLayers; i++)
	{
		fprintf(file, "\tLA%d:\n", i);
		for (int j = 0; j < n.layersSize[i]; j++)
		{
			fprintf(file, "\t\tNE%d:\n", j);
			writeNeuron(n.neurons[i][j], file);
		}
	}
	return 1;
}

int SWrite(Network n, char* fileName)
{
	if (fopen(fileName, "r"))
		remove(fileName);
	FILE *file = fopen(fileName, "w");
	if (file)
	{
		writeNetwork(n, file);
		fclose(file);
	}
	else
	{
		printf("Erreur lors de l'ouverture du fichier");
		return 0;
	}
	return 1;
}

int readNetwork(FILE *file, Network *n)
{
	int error = 1;
	fseek(file, 5, ftell(file));
	if (error != EOF)
		error = fscanf(file, "\tNBL: %d\n", &(n->nbLayers));
	else
		return error;
	n->layersSize = malloc(sizeof(int)*(n->nbLayers));
	for (int i = 0; i < n->nbLayers; i++)
	{
		if (error != EOF)
		{
			error =fscanf(file, "\t\tLA%d: %d\n", &i, &(n->layersSize[i]));
		}
		else
			return error;
	}
	if (error != EOF)
		error = fscanf(file, "\tBIA: %d\n", &(n->bias));
	else
		return error;
	n->neurons = malloc(sizeof(Neuron*));
	for (int i = 0; i < n->nbLayers; i++)
	{
		error = fscanf(file, "\tLA%d:\n", &i);
//		fseek(file, 6, ftell(file));
		n->neurons[i] = malloc(sizeof(Neuron)*(n->layersSize[i]));
		for (int j = 0; j < n->layersSize[i]; j++)
		{
			error = fscanf(file, "\t\tNE%d:\n", &j);
			if (error != EOF)
				error = readNeuron(file, &(n->neurons[i][j]));
			else
				return error;
		}
	}
	return error;
}

int readNeuron(FILE *file, Neuron *n)
{
	int error = 1;
	if (error != EOF)
	{
		int fooID;
		error = fscanf(file, "\t\t\tFOO: %d\n", &fooID);
		n->shockFoo = fooID;
	}
	else
		return error;
	if (error != EOF)
		error = fscanf(file, "\t\t\tNBC: %d\n", &(n->nbConnections));
	else
		return error;
	n->connectList = malloc(sizeof(Connection)*(n->nbConnections));
	for(int i = 0; i < n->nbConnections; i++)
	{
		error = fscanf(file, "\t\t\tCO%d:\n", &i);
		error = readConnection(file, &(n->connectList[i]));
	}
	return error;
}

int readConnection(FILE *file, Connection *c)
{
	int error = 1;
	if (error != EOF)
	error = fscanf(file, "\t\t\t\tLAY: %d\n", &(c->layer));
	else
		return error;
	if (error != EOF)
		error = fscanf(file, "\t\t\t\tIND: %d\n", &(c->index));
	else
		return error;
	if (error != EOF)
	{
		error = fscanf(file, "\t\t\t\tWEI: %lf\n", &(c->weight));
		printf("saved %lf\n", c->weight);
	}
	else
		return error;
	return error;
}

Network SRead(char *fileName)
{
	Network n;
	int error = 1;
	FILE *file = fopen(fileName, "r");
	if (file)
	{
		error = readNetwork(file, &n);
		fclose(file);
	}
	else
	{
		printf("Erreur lors de l'ouverture du fichier");
		return n;
	}
	if (error == EOF)
		printf("Erreur lors de l'écriture du fichier");
	return n;
}
