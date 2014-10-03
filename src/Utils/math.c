#include "math.h"
#include <math.h>

int UPow(int a, int b)
{
	int tmp = 1;
	for (int i = 0; i < b; i ++)
		tmp *= a;
	return tmp;
}

double UComputeError(double *target, double *output, int size)
{
	double r = 0;
	for (int i = 0; i < size; i ++)
		r += (target[i] - output[i]) * (target[i] - output[i]);
	return r / size;
}

double UComputeShock(double sum)
{
	return 1 / (1 + exp(-sum));
}
