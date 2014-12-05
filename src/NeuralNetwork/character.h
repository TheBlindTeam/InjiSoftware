#ifndef CHARACTERNEURAL_H
#define CHARACTERNEURAL_H
#include "../Utils/image.h"
#include "../PreTreatment/segmentation.h"
#include "stdlib.h"
#include "neuronsUtils.h"
#include "learning.h"

extern const int charInputSize;

CharOutput *Recognize(NetworkSet *nWorkSet, double *input, int *size);

void NInitCharacterNetworkSetParams(NetworkSet *ref);

NetworkSet* NInitCharacterNetworkSet(char *path);

ExempleSet *NGetCharExempleSet(char *path);

gunichar ConvertToOrderedChar(gunichar c);

gunichar ConvertToRegularChar(gunichar c);

double *ConvertCharToTargetArray(gunichar c);

ImageBN *ToSquareImage(ImageBN *img, struct Box *b);

ImageBN *ResizeImageBNToChar(ImageBN *img);

double *ConvertImageToInput(ImageBN *img);
#endif
