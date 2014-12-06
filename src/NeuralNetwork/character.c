#include "character.h"

const int charInputSize = 32;
const int outputSize = 162;//A Changer
const double learningRate = 0.03;
const double momentum = 0.1;
const double overfitCoef = 0.08;

int compareCharOutput(const void *a, const void *b)
{
	double tmp = ((CharOutput*)a)->prob - ((CharOutput*)b)->prob;
	if (tmp > 0)
		return 1;
	if (tmp < 0)
		return -1;
	return 0;
}

CharOutput *Recognize(NetworkSet *nWorkSet, double *input, int *size)
{
	CharOutput *r;
	double *output;
	*size = 0;
	NRun(nWorkSet->nWork, input, &output);
	for (int i = 0; i < outputSize; i ++)
		if (output[i] >= 0.8)
			(*size)++;
	r = malloc(sizeof(CharOutput) * (*size));
	*size = 0;
	for (int i = 0; i < outputSize; i ++)
		if (output[i] >= 0.8)
		{
			r[i].c = ConvertToRegularChar((gchar)i);
			r[i].prob = output[i];
			(*size)++;
		}
	qsort(r, *size, sizeof(CharOutput), compareCharOutput);
	return r;
}

void NInitCharacterNetworkSetParams(NetworkSet *ref)
{
	ref->maxError = 0;
	ref->lRate = learningRate;
	ref->momentum = momentum;
	ref->overfitCoef = overfitCoef;
	ref->learn = &NBackPropLearn;
}

NetworkSet* NInitCharacterNetworkSet(char *path)
{
	NetworkSet *r = malloc(sizeof(NetworkSet));
	NInitCharacterNetworkSetParams(r);
	if (!path)
	{
		r->nWork = NINIT[5](charInputSize * charInputSize, outputSize);
		NInitThresHoldSimpleMLP(r->nWork, LINEAR, LINEAR, LINEAR, TAN_SIGMOID);
	}
	else
		r->nWork = SRead(path);
	return r;
}

ExempleSet *NGetCharExempleSet(char *path)
{
	FILE* fp = fopen(path, "r");
	if (fp)
	{
		int c;
		int nbLines = 0;
		while((c = getc(fp)) != EOF)
			if(c == '\n')
				nbLines++;
		rewind(fp);
		double **input;
		double **target;
		input = malloc(sizeof(double*) * nbLines);
		target = malloc(sizeof(double*) * nbLines);
		for (int i = 0; i < nbLines; i ++)
		{
			target[i] = ConvertCharToTargetArray(getc(fp));
			c = getc(fp);
			input[i] = malloc(sizeof(double) * charInputSize * charInputSize);
			for (int j = 0; j < charInputSize * charInputSize; j ++)
				input[i][j] = (double)getc(fp);
			while((c = getc(fp)) != '\n');
		}
		ExempleSet *r = NGetExempleSet(input, charInputSize * charInputSize, target, outputSize, nbLines);
		fclose(fp);
		return r;
	}
	printf("Error opening file - NGetCharExempleSet\n");
	return NULL;
}

gunichar ConvertToOrderedChar(gunichar c)
{
	if(c >= 33 && c <= 126)
		return c-33;
	if(c >= 188 && c <= 255)
		return c-94;
	return 255;
}

gunichar ConvertToRegularChar(gunichar c)
{
	if(c <= 93)
		return c+33;
	return c+94;
}


double *ConvertCharToTargetArray(gunichar c)
{
	int tmp = ConvertToOrderedChar(c);
	if (tmp < 255)
	{
		int size = ConvertToOrderedChar(255) + 1;
		double *r = malloc(sizeof(double) * size);
		for (int i = 0; i < size; i ++)
			r[i] = 0;
		r[tmp] = 1;
		return r;
	}
	return NULL;
}

ImageBN *ToSquareImage(ImageBN *img, Box *b)
{
	ImageBN *r = malloc(sizeof(ImageBN));
	int size_y = b->rectangle.y2 - b->rectangle.y1 + 1;
	int size_x = b->rectangle.x2 - b->rectangle.x1 + 1;
	r->width = size_y > size_x ? size_y : size_x;
	r->height = r->width;
	r->data = malloc(sizeof(int*) * r->width);
	for (int i = 0; i < r->width; i++)
	{
		r->data[i] = malloc(sizeof(int) * r->height);
		for (int j = 0; j < r->height; j++)
			r->data[i][j] = 0;
	}
	for (int i = b->rectangle.x1; i <= b->rectangle.x2; i ++)
		for (int j = b->rectangle.y1; j <= b->rectangle.y2; j++)
		{
			r->data[i + (r->width - size_x) / 2 - b->rectangle.x1]
				[j + (r->height - size_y) / 2 - b->rectangle.y1] = img->data[i][j];
		}
	return r;
}

ImageBN *ResizeImageBNToChar(ImageBN *img)
{
	ImageBN *r = malloc(sizeof(ImageBN));
	r->width = charInputSize;
	r->height = charInputSize;
	r->data = malloc(sizeof(int*) * r->width);
	for (int i = 0; i < r->width; i ++)
	{
		r->data[i] = malloc(sizeof(int) * r->height);
		for (int j = 0; j < r->height; j ++)
			r->data[i][j] = 0;
	}
	if (img->width > 1 && img->height > 1)
	{
		for (int i = 0; i < img->width; i ++)
			for (int j = 0; j < img->height; j ++)
				if (img->data[i][j])
				{
					int x = round((double)(r->width - 1) * (double)i / (double)(img->width - 1));
					int y = round((double)(r->height - 1) * (double)j / (double)(img->height - 1));
					r->data[x][y] = 1;
				}
		for (int i = 0; i < r->width; i ++)
		{
			for (int j = 0; j < r->height; j ++)
			{
				int x = round((double)(img->width - 1) * (double)i / (double)(r->width - 1));
				int y = round((double)(img->height - 1) * (double)j / (double)(r->height - 1));
				if(img->data[x][y])
					r->data[i][j] = img->data[x][y];
			}
		}
	}
	return r;
}

double *ConvertImageToInput(ImageBN *img)
{
	if (img->width != charInputSize || img->height != charInputSize)
		return NULL;
	double *r = malloc(sizeof(double) * charInputSize * charInputSize);
	for (int i = 0; i < img->width; i++)
		for (int j = 0; j < img->height; j ++)
			r[i * img->height + j] = img->data[i][j];
	return r;
}
