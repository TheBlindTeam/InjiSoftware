#include <math.h>
#ifndef _MATHUTILS
#define _MATHUTILS
typedef enum
{
	LINEAR = 0,
	SIGMOID = 1,
	TAN_SIGMOID = 2
} FunctionId;

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

extern const FAndDifF FUNCTIONS[];
#endif
