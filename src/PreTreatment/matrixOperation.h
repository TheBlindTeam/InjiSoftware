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
void ClampPixel(Pixel *pix, int min, int max)
{
	pix->r = (pix->r <= min) ? min : pix->r;
	pix->r = (pix->r >= max) ? max : pix->r;

	pix->g = (pix->g <= min) ? min : pix->g;
	pix->g = (pix->g >= max) ? max : pix->g;

	pix->b = (pix->b <= min) ? min : pix->b;
	pix->b = (pix->b >= max) ? max : pix->b;
}

// Modify the position of a point according to a angle of rotation.
Vector2 ApplyVectorRot(Vector2 origin, double radian);

// Get min and max values from several vectors
void ExtremumVectorValues(Vector2 *tab, int arraySize, 
	Vector2 *min, Vector2 *max);

// David: Multiply Pixel Matrix By Convolution Matrix
Pixel** UMultiply(Pixel **matrix, double **convolution, int matrixSize);

// David: Extract Submatrix From The Center
Pixel** UExtract(Pixel **matrix, int matrixSize, int extractSize, int pos_x,
	int pos_y);

// David: Apply A Convulution Matrix On Every Pixel
Image UConvolution(Image ref, double **convolution, int size,
	int matrixSize);

// David : Rotate A Given Matrix
Image URotate(Image ref, double angle);

#endif
