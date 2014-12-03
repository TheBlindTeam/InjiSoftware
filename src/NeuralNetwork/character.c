#include "character.h"

const int charInputSize = 16;

unsigned char ConvertToOrderedChar(wchar_t c)
{
	if (c >= 0x21 && c <= 0x7e)
		return c - 0x21;
	if (c >= 0xa1 && c <= 0xac)
		return c - 0xa1 + (0x7e - 0x21 + 1);
	if (c >= 0xae)
		return c - 0xae + (0xac - 0xa1 + 1) + (0x7e - 0x21 + 1);
	return 255;
}

unsigned char ConvertToUtf8(wchar_t c)
{
	if (c <= 0x7e - 0x21)
		return c + 0x21;
	if (c <= 0xac - 0xa1 + (0x7e - 0x21 + 1))
		return c + 0xa1 - (0x7e -0x21 + 1);
	return c + 0xae - (0xac - 0xa1 + 1) - (0x7e + 0x21 + 1);
}

double *ConvertCharToTargetArray(wchar_t c)
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
				[j - b->rectangle.y1] = img->data[i][j];
	return r;
}

ImageBN *ResizeImage(ImageBN *img)
{
	//FIXME
	img = img;
	return NULL;
}

double *ConvertImageToInput(ImageBN *img)
{
	if (img->width != charInputSize || img->height != charInputSize)
		return NULL;
	double *r = malloc(sizeof(double) * charInputSize * charInputSize);
	for (int i = 0; i < img->width; i++)
		for (int j = 0; j < img->height; j ++)
		{
			r[i * img->height + j] = img->data[i][j];
		}
	return r;
}
