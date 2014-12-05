#ifndef CHARACTERNEURAL_H
#define CHARACTERNEURAL_H
#include "../Utils/image.h"
#include "../PreTreatment/segmentation.h"
#include "stdlib.h"

extern const int charInputSize;

gchar ConvertToOrderedChar(gchar c);

gchar ConvertToRegularChar(gchar c);

double *ConvertCharToTargetArray(gchar c);

ImageBN *ToSquareImage(ImageBN *img, struct Box *b);

ImageBN *ResizeImageBNToChar(ImageBN *img);

double *ConvertImageToInput(ImageBN *img);
#endif
