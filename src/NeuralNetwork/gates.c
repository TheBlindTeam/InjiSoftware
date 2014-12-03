#include "gates.h"

ExempleSet *NGetAndExempleSet()
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
	ExempleSet *r = NGetExempleSet(input, 2, target, 1, 4);
	for (int i = 0; i < 4; i ++)
	{
		free(input[i]);
		free(target[i]);
	}
	free(input);
	free(target);
	return r;
}

ExempleSet *NGetOrExempleSet()
{
	double **input = malloc(sizeof(double) * 4);
	double **target = malloc(sizeof(double) * 4);
	for (int i = 0; i < 4; i ++)
	{
		input[i] = malloc(sizeof(double) * 2);
		target[i] = malloc (sizeof(double));
		input[i][0] = i % 2;
		input[i][1] = i / 2;
		target[i][0] = i ? 1 : 0;
	}
	ExempleSet *r = NGetExempleSet(input, 2, target, 1, 4);
	for (int i = 0; i < 4; i ++)
	{
		free(input[i]);
		free(target[i]);
	}
	free(input);
	free(target);
	return r;
}

ExempleSet *NGetXorExempleSet()
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
	ExempleSet *r = NGetExempleSet(input, 2, target, 1, 4);
	for (int i = 0; i < 4; i ++)
	{
		free(input[i]);
		free(target[i]);
	}
	free(input);
	free(target);
	return r;
}

NetworkSet* NInitGatesNetworkSet(int gate, int archi, int learning,
	FunctionId input, FunctionId output, FunctionId others, FunctionId bias)
{
	NetworkSet* r = malloc(sizeof(NetworkSet));

	r->maxError = 0.000001;
	r->lRate = 0.06;
	r->momentum = 0.2;
	r->overfitCoef = 0.1;
	if (gate == 0)
		r->exSet = NGetAndExempleSet();
	else if (gate == 1)
		r->exSet = NGetOrExempleSet();
	else
		r->exSet = NGetXorExempleSet();
	if (learning == 0)
	{
		Network *N1 = NINIT[archi](r->exSet->inputSize,
			r->exSet->targetSize);
		Network *N2 = NINIT[archi](r->exSet->inputSize,
			r->exSet->targetSize);

		NInitThresHoldSimpleMLP(N1, input,
			output, bias, others);
		NInitThresHoldSimpleMLP(N2, input,
			output, bias, others);
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
			r->nWork = NINIT[archi](r->exSet->inputSize,
				r->exSet->targetSize);
			NInitThresHoldSimpleMLP(r->nWork, input,
				output, bias, others);
			NComputeError(r->nWork, r->exSet, 0, NULL, 0);
			r->learn = &NBackPropLearn;
	}
	return r;
}

