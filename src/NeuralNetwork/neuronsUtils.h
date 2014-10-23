#ifndef _NEURONSUTILS
#define _NEURONSUTILS
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

typedef struct Network
{
	int nbLayers;
	int *layersSize;
	int bias;
	double error;
	Neuron **neurons;
	struct Network *sibling;
} Network;

typedef struct Exemple
{
	double *input;
	double *target;
	struct Exemple *next;
} Exemple;

typedef struct
{
	int inputSize;
	int targetSize;
	Exemple *exemple;
} ExempleSet;

typedef struct NetworkSet
{
	int (*learn)(struct NetworkSet *nWorkSet);
	Network *nWork;
	double maxError;
	double lRate;
	double momentum;
	ExempleSet exSet;
} NetworkSet;

Network *NInitializeSimpleMLP(int input, int output, int middle, int bias);

Network *NInitializeCompleteBias(int input, int output);

Network *NInitializeCompleteNBias(int input, int output);

Network *NInitializeLinearBias(int input, int output);

Network *NInitializeLinearNBias(int input, int output);

void NInitThresHoldSimpleMLP(Network *nWork, FAndDifF input, FAndDifF output,
	FAndDifF bias, FAndDifF others);

void NInitializeSumNetwork(Network *nWork);

int NRun(Network *nWork, double *input, double **r);

double NComputeError(Network *nWork, ExempleSet exSet, int print, char** str);

void NPrintNetwork(Network nWork);

void NInitEdge(Network *nWork, int startL, int startI, int endL, int endI, int k);

void addInExempleSet(ExempleSet *exSet, double *input, int inputSize,
	double *target, int targetSize);

ExempleSet NGetXorExempleSet();

NetworkSet NInitNetworkSet(int *argv);

NetworkSet NDefaultNetworkSet();

int specialPrint(char *s);

int getMaxNeuronsLayer(Network nWork);
int getTotalNeuronsAmount(Network nWork);
#endif
