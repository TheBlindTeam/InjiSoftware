#include "skeleton.h"

ImageBN *PreTreatment(Image *ref)
{
	Image *tmp=ref;
	Image *tmpForFree=NULL;
	ImageBN *tmpBn = URgbToBinary(ref);
	//double angle = FindInclinationAngle(tmpBn);
	UFreeImageBinary(tmpBn);
	/*if ((int)angle)
	{
		tmp = URotate(tmp, angle);
		
			//Enlever les commentaires quand il y aura l'effacement du bruit
		//free(tmpForFree);
		
		tmpForFree = tmp;
		
	}*/
	tmpBn = URgbToBinary(tmp);
	if (tmpForFree) //enlever quand le lissage sera fait
		UFreeImage(tmpForFree);
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
