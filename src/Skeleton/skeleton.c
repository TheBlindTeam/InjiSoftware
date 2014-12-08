#include "skeleton.h"

ImageBN *PreTreatment(Image *ref)
{
	Image *tmp = NULL;
	ImageBN *tmpBn = URgbToBinary(ref);
	double angle = 0;//FindInclinationAngle(tmpBn);
	UFreeImageBinary(tmpBn);
	if ((int)angle)
	{
		tmp = URotate(ref, angle);
		tmpBn = URgbToBinary(tmp);
		UFreeImage(tmp);
	}
	else
		tmpBn = URgbToBinary(ref);
	return tmpBn;
}

void RecognizeAllCharacters(Box *b, NetworkSet *n)
{
	if (b->lvl == CHARACTER)
		b->output = Recognize(n, b->input, &b->nbOutput);
	for (int i = 0; i < b->nbSubBoxes; i ++)
		RecognizeAllCharacters(b->subBoxes[i], n);
}

Box *Recognition(NetworkSet *nWorkSet, ImageBN* imgBn)
{
	Image *img = UBinaryToRgb(imgBn);
	Box *r = GetBoxFromSplit(img, img);
	RecognizeAllCharacters(r, nWorkSet);
	UFreeImage(img);
	return r;
}
