#include <stdio.h>
#include <stdlib.h>
#include "../NeuralNetwork/neuronsUtils.h"

int writeNetwork(Network network, FILE *file);
int writeNeuron(Neuron neuron, FILE *file);
int writeConnection(Connection ct, FILE *file);

/*Ecrit dans un .inji les informations sur le réseau
de neurones et retourne le nom de fichier*/
int SWrite(Network n, char* fileName);
/*Lit le fichier passé en paramètres et retourne le réseau correspondant*/
int readNetwork(FILE *file, Network *n);
int readNeuron(FILE *file, Neuron *n);
int readConnection(FILE *file, Connection *c);
Network *SRead(char* fileName);
