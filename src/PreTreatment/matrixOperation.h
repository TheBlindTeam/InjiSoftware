#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// David: Multiply Pixel Matrix By Convolution Matrix
Pixel** UMultiply(Pixel **matrix, double **convolution, int matrixSize);

// David: Extract Submatrix From The Center
Pixel** UExtract(Pixel **matrix, int matrixSize, int extractSize, int pos_x,
	int pos_y);

// David: Apply A Convulution Matrix On Every Pixel
Pixel** UConvolution(Pixel **matrix, double **convolution, int size,
	int matrixSize);

// David : Rotate A Given Matrix
Image URotate(Image ref, double angle);
