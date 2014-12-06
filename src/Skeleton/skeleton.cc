#include "skeleton.h"

ImageBN *PreTreatment(Image *ref)
{
	Image *tmpForFree = NULL;
	Image *tmp;
	tmp = ref; //A remplacer par l'effacement du bruit
	/*
		//ENelever les commentaires quand le lissage sera fait
	tmpForFree = tmp;
	*/
	double angle = FindInclinationAngle(tmp);
	if ((int)angle)
	{
		tmp = URotate(tmp, angle);
		/*
			//Enlever les commentaires quand il y aura l'effacement du bruit
		free(tmpForFree);
		*/
		tmpForFree = tmp;
	}
	ImageBN *tmpBn = UrgbToBinary(tmp);
	if (tmpForFree) //enlever quand le lissage sera fait
		free(tmpForFree);
	return tmpBn;
}

void RecognizeAllCharacters(Box *b, NetworkSet *n)
{
	if (b->lvl = CHARACTER)
		b->Recognize(n, b->input, &b->nbOutput);
	for (int i = 0; i < b->nbSubBoxes; i ++)
		RecognizeAllCharacters(b->subBoxes[i]);
}

Box *Recognition(NetworkSet *nWorkSet, ImageBN* imgBn)
{
	Image *img = UBinaryToRgb(imgBn);
	Box *r = GetBoxFromSplit(img, img);
	RecognizeAllCharacters(r, nWorkSet);
	UFreeImage(img);
	return r;
}
