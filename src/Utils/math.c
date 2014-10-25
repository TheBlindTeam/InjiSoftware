#include "math.h"
#include <stdio.h>

const FAndDifF SIGMOID = {&USigmoid, &UDSigmoid};
const FAndDifF TAN_SIGMOID = {&UTanSigmoid, &UDTanSigmoid};
const FAndDifF LINEAR = {&ULinear, &UDLinear};
const FAndDifF FUNCTIONS[] = { {&ULinear, &UDLinear}, {&USigmoid, &UDSigmoid}, {&UTanSigmoid, &UDTanSigmoid} };

int UPow(int a, int b)
{
	int tmp = 1;
	for (int i = 0; i < b; i ++)
		tmp *= a;
	return tmp;
}

double USquarredError(double *target, double *output, int size)
{
	double r = 0;
	for (int i = 0; i < size; i ++)
		r += (target[i] - output[i]) * (target[i] - output[i]);
	return r / size;
}

double USigmoid(double sum)
{
	return 1 / (1 + exp(-sum));
}

double UDSigmoid(double sum)
{
	return USigmoid(sum) * (1 - USigmoid(sum));
}

double ULinear(double sum)
{
	return sum;
}

double UDLinear(double sum)
{
	sum ++;
	return 1;
}

double UTanSigmoid(double sum)
{
	return tanh(sum);
}

double UDTanSigmoid(double sum)
{
	return 1 - UTanSigmoid(sum) * UTanSigmoid(sum);
}
