#ifndef MATRIXOPERATION_H
#define MATRIXOPERATION_H

#include "../Utils/image.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct c_vector2{

	int x,y;

}Vector2;

// David : Simple clamp function applied on all channels of a given Pixel
void ClampPixel(Pixel *pix, int min, int max);

// Modify the position of a point according to a angle of rotation.
Vector2 ApplyVectorRot(Vector2 origin, double radian);

// Get min and max values from several vectors
void ExtremumVectorValues(Vector2 *tab, int arraySize, 
	Vector2 *min, Vector2 *max);

// David: Multiply Pixel Matrix By Convolution Matrix and return the middle pixel
Pixel UConvolutionProduct(Pixel **matrix, double **convolution, int matrixSize);

// David: Extract Submatrix From The Center
Pixel** UExtract(Pixel **matrix, int wSize, int hSize, int extractSize, int pos_x,
	int pos_y);

// David: Apply A Convulution Matrix On Every Pixel
Image *UConvolution(Image *ref, double **convolution, int matrixSize);

// David : Rotate A Given Matrix
Image *URotate(Image *ref, double angle);

#endif
