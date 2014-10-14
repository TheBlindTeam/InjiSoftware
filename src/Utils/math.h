#include <math.h>
#ifndef _MATHUTILS
#define _MATHUTILS
typedef struct
{
	double (*f)(double);
	double (*df)(double);
}FAndDifF;


int UPow(int a, int b);

double USquarredError(double *target, double *output, int size);

double USigmoid(double sum);
double UDSigmoid(double sum);
double ULinear(double sum);
double UDLinear(double sum);
double UTanSigmoid(double sum);
double UDTanSigmoid(double sum);


extern const FAndDifF SIGMOID;
extern const FAndDifF TAN_SIGMOID;
extern const FAndDifF LINEAR;

#endif
