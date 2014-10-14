#include "../Utils/math.h"

extern const double RAND_UP;
extern const double RAND_DOWN;

typedef struct
{
	int layer;
	int index;
	double weight;
	double prevChange;
} Connection;

typedef struct
{
	double sum;
	double shock;
	FAndDifF shockFoo;
	int nbConnections;
	double error;
	Connection *connectList;
} Neuron;

typedef struct
{
	int nbLayers;
	int *layersSize;
	int bias;
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

Network NInitializeCompleteNetwork(ExempleSet exSet, FAndDifF shockFoo);

void NInitializeSumNetwork(Network nWork);

double *NRun(Network nWork, double *input);

double NComputeSquarredError(Network nWork, ExempleSet exSet, int needPrint);

void NPrintNetwork(Network nWork);

Network NGetDichotomicTrainingNetwork(ExempleSet exSet, double maxError);

void NChangeLostNetwork(Network won, double wError, Network lost, double lError);

void NInitEdge(Network nWork, int startL, int startI, int endL, int endI, int k);


Network NGetBackPropTrainingNetwork(ExempleSet exSet, double maxError, double momentum, double lRate);
