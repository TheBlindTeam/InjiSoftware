#ifndef MATRIXOPERATION_H
#define MATRIXOPERATION_H

#include "../Utils/image.h"
#include "../Utils/sort.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct c_vector2{

	int x,y;

}Vector2;

// Simple clamp function applied on all channels of a given Pixel
void ClampPixel(Pixel *pix, int min, int max);

int ClampDouble(double a, int min, int max);

// Modify the position of a point according to an angle of rotation.
Vector2 ApplyVectorRot(Vector2 origin, double radian);

// Get min and max values from several vectors
void ExtremumVectorValues(Vector2 *tab, int arraySize,
	Vector2 *min, Vector2 *max);

// Apply A Convolution Matrix On Every Pixel
Image *UConvolution(Image *ref, double **convolution, int matrixSize);

//- - - - - Matrix Operation on Grayscale IMG - - - - -//
// Apply Median filter on Grayscale image and return the new image
ImageGS *MedianFilter(ImageGS *ref, size_t filterSize);

// Extract 2D Matrix with a given size to a 1-Dimensionnal array
// Size will be modified according to the new size of the 1-D array
guchar *ExtractNeighbors(ImageGS *ref, int posX, int posY, int *size);

Image *UCutAlphaMargin(Image *ref);

// Rotate A Given Matrix
Image *URotate(Image *ref, double angle);

#endif
