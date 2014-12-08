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
	printf("b->lvl %d\n", b->lvl);
	if (b->lvl == CHARACTER)
	{
		printf("Avant %p\n", b->input);
		b->output = Recognize(n, b->input, &b->nbOutput);
		printf("Apres\n");
	}
	for (int i = 0; i < b->nbSubBoxes; i ++)
	{
		printf("boucle\n");
		RecognizeAllCharacters(b->subBoxes[i], n);
	}
}

Box *Recognition(NetworkSet *nWorkSet, ImageBN* imgBn)
{
	printf("Et la ?\n");
	Image *img = UBinaryToRgb(imgBn);
	printf("Et la ?\n");
	Box *r = GetBoxFromSplit(img, img);
	printf("Et la ?\n");
	RecognizeAllCharacters(r, nWorkSet);
	printf("Et la ?\n");
	UFreeImage(img);
	printf("Et la ?\n");
	return r;
}
