#ifndef SKELETON_H
#define SKELETON_H

#include <stdlib.h>
#include <stdio.h>
#include "../PreTreatment/matrixOperation.h"
#include "../PreTreatment/segmentation.h"
#include "../PreTreatment/hough.h"
#include "../Utils/image.h"
#include "../NeuralNetwork/neuronsUtils.h"

// Filtering - Binarization - Rotation - Segmentation - Recognition
Box *Recognition(NetworkSet *nWorkSet, ImageBN* imgBn);

ImageBN *PreTreatment(Image *ref);

void RecognizeAllCharacters(Box *b, NetworkSet *n);
#endif
