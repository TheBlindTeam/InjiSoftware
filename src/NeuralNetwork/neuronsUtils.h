#ifndef NEURONSUTILS
#define NEURONSUTILS

const double RAND_UP = 1;
const double RAND_DOWN = -1;
typedef struct
{
	int layer;
	int index;
	double weight;
} Connection;

typedef struct
{
	double sum;
	double shock;
	int nbConnections;
	Connection *connectList;
} Neuron;

typedef struct
{
	int nbLayers;
	int *layersSize;
	Neuron **neurons;
} Network;

typedef struct
{
	double *input;
	double *target;
} Exemple;

typedef struct
{
	int size;
	int inputSize;
	int targetSize;
	Exemple *exempleList;
} ExempleSet;

Network NInitializeCompleteNetwork(ExempleSet exSet);

void NInitializeSumNetwork(Network nWork);

double *NRun(Network nWork, double *input);

double NComputeError(Network nWork, ExempleSet exSet, int needPrint);

void NPrintNetwork(Network nWork);

Network NGetTrainedNetwork(ExempleSet exSet, double maxError);

void NChangeLostNetwork(Network won, double wError, Network lost, double lError);
#endif
