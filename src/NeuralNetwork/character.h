#include "../Utils/image.h"
#include "../PreTreatment/segmentation.h"
#include "stdlib.h"

extern const int charInputSize;

unsigned char ConvertToOrderedChar(unsigned char c);

unsigned char ConvertToUtf8(unsigned char c);

double *ConvertCharToTargetArray(unsigned char c);

ImageBN *ToSquareChar(ImageBN *img, Box *b);

ImageBN *ResizeChar(ImageBN *img);

double *ConvertImageToInput(ImageBN *img);
