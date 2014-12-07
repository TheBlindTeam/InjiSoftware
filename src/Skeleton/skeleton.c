#include "skeleton.h"

ImageBN *PreTreatment(Image *ref)
{
	printf("Et la 1?\n");
	Image *tmp;
	printf("Et la 2\n");
	Image *tmpForFree;
	printf("Et la 3?\n");
	tmp = ref; //A remplacer par l'effacement du bruit
	printf("Et la 4?\n");
	tmpForFree = tmp;
	printf("Et la 5?\n");
	ImageBN *tmpBn = URgbToBinary(tmp);
	printf("Et la 6?\n");
	double angle = FindInclinationAngle(tmpBn);
	printf("Et la 7?\n");
	UFreeImageBinary(tmpBn);
	printf("Et la 8?\n");
	if ((int)angle)
	{
		tmp = URotate(tmp, angle);
		/*
			//Enlever les commentaires quand il y aura l'effacement du bruit
		free(tmpForFree);
		*/
		tmpForFree = tmp;
		
	}
	printf("Et la 10?\n");
	tmpBn = URgbToBinary(tmp);
	printf("Et la 11?\n");
	if (tmpForFree) //enlever quand le lissage sera fait
		UFreeImage(tmpForFree);
	printf("Et la 12?\n");
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
