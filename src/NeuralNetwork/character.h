#include "../Utils/image.h"
#include "../PreTreatment/segmentation.h"
#include "stdlib.h"

extern const int charInputSize;

unsigned char ConvertToOrderedChar(wchar_t c);

unsigned char ConvertToUtf8(wchar_t c);

double *ConvertCharToTargetArray(wchar_t c);

ImageBN *ToSquareImage(ImageBN *img, Box *b);

ImageBN *ResizeImage(ImageBN *img);

double *ConvertImageToInput(ImageBN *img);
