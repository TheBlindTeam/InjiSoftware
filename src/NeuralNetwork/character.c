#include "character.h"

const int charInputSize = 32;
const int output = 255;
const double learningRate = 0.03;
const double momentum = 0.1;
const double overfitCoef = 0.08;

/*CharOutput *()

ExempleSet *NGetCharExempleSet(char *path)
{
	
}*/

gchar ConvertToOrderedChar(gchar c)
{
	return c;
}

gchar ConvertToRegularChar(gchar c)
{
	
	return c;
}


double *ConvertCharToTargetArray(gchar c)
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
			r->data[i + (r->width - size_x) / 2 - b->rectangle.x1]
				[j + (r->height - size_y) - b->rectangle.y1] = img->data[i][j];
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
	//if (r->width < img->width)
		for (int i = 0; i < img->width; i ++)
			for (int j = 0; j < img->height; j ++)
				if (img->data[i][j])
				{
					int x = round((double)(r->width - 1) * (double)i / (double)(img->width - 1));
					int y = round((double)(r->height - 1) * (double)j / (double)(img->height - 1));
					r->data[x][y] = 1;
				}
	//else
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
