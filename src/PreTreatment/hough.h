#ifndef HOUGH_H
#define HOUGH_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include"../Utils/image.h"

// David : Gives the position (the x position) of the maximum in a given array
int GetMaxIndex(int **array, int sizeX, int sizeY);

// David : Allows us to find an angle to put the right image orientation
double FindInclinationAngle(Image ref);
#endif
