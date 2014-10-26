#include "image.h"
#include <stdlib.h>
#include <stdio.h>

const Pixel RED = {255, 0, 0, 255};
const Pixel GREEN = {0, 255, 0, 255};
const Pixel BLUE = {0, 0, 255, 255};

Image* ULoadImage(char *str)
{
	Image *tmp = malloc(sizeof(Image));
	GError *error = NULL;

	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(str, &error);

	if (!pixbuf || error)
	{
		tmp->width = 0;
		tmp->height = 0;
		tmp->has_alpha = FALSE;
		tmp->pixList = NULL;
		printf("%d : %s\n", error->code, error->message);
	}
	else
	{
		tmp->width = gdk_pixbuf_get_width(pixbuf);
		tmp->height = gdk_pixbuf_get_height(pixbuf);
		tmp->has_alpha = gdk_pixbuf_get_has_alpha(pixbuf);
		tmp->bits_per_sample = gdk_pixbuf_get_bits_per_sample(pixbuf);
		tmp->pixList = malloc(sizeof(Pixel*) * tmp->width);
		for (int i = 0 ; i < tmp->width; i ++)
		{
			tmp->pixList[i] = malloc(sizeof(Pixel) *
					tmp->height);
			for (int j = 0; j < tmp->height; j ++)
				tmp->pixList[i][j] = UGetPixel(pixbuf, i, j);
		}
	}
	return tmp;
}

void UFreeImage(Image *img)
{
	for (int i = 0; i < img->width; i ++)
		free(img->pixList[i]);
	free(img->pixList);
	free(img);
}

void UFreeImageGray(ImageGS *img)
{
	for (int i = 0; i < img->width; i ++)
		free(img->intensity[i]);
	free(img->intensity);
	free(img);
}

void UFreeImageBinary(ImageBN *img)
{
	for (int i = 0; i < img->width; i ++)
		free(img->data[i]);
	free(img->data);
	free(img);
}

Pixel UGetPixel(GdkPixbuf *pixbuf, int x, int y)
{
	Pixel pixel;

	guchar *pixelsBuf = gdk_pixbuf_get_pixels(pixbuf);
	gint channel = gdk_pixbuf_get_n_channels(pixbuf);
	gint rowstride = gdk_pixbuf_get_rowstride(pixbuf);

	int idx = y * rowstride + x * channel;
	pixel.r = pixelsBuf[idx];
	pixel.g = pixelsBuf[idx + 1];
	pixel.b = pixelsBuf[idx + 2];
	if (channel == 4)
		pixel.a = pixelsBuf[idx + 3];
	else
		pixel.a = 255;
	return pixel;
}

ImageGS *URgbToGrayscale(Image *rgbImage)
{
	ImageGS *result = malloc(sizeof(ImageGS));
	result->width = rgbImage->width;
	result->height = rgbImage->height;
	result->intensity = malloc(result->width * sizeof(guchar*));

	for(int x = 0; x < result->width; x++)
	{
		result->intensity[x] = malloc(result->height * sizeof(guchar));
		for (int y = 0; y < result->height; y ++)
		{
			result->intensity[x][y] = (
				(0.3 * (double)rgbImage->pixList[x][y].r) +
				(0.59 * (double)rgbImage->pixList[x][y].g) +
				(0.11 * (double)rgbImage->pixList[x][y].b)) + 0.5;
		}
	}
	return result;
}

Image *UGrayscaleToRgb(ImageGS *reference)
{
	Image *result = malloc(sizeof(Image));
	result->width = reference->width;
	result->height = reference->height;
	result->has_alpha = 1;
	result->bits_per_sample = 8;
	result->pixList = malloc(result->width * sizeof(Pixel*));

	for (int x = 0; x < result->width; x++)
	{
		result->pixList[x] = malloc(result->height * sizeof(Pixel));
		for(int y = 0; y < result->height; y++)
		{
			result->pixList[x][y].r = reference->intensity[x][y];
			result->pixList[x][y].g = reference->intensity[x][y];
			result->pixList[x][y].b = reference->intensity[x][y];
			result->pixList[x][y].a = 255;
		}
	}
	return result;
}

ImageBN *UGrayscaleToBinary(ImageGS *ref)
{
	ImageBN *result = malloc(sizeof(ImageBN));
	result->width = ref->width;
	result->height = ref->height;

	result->data = malloc(result->width * sizeof(int*));
	for (int x = 0; x < result->width; x++)
	{
		result->data[x] = malloc(result->height * sizeof(int));
		for (int y = 0; y < result->height; y++)
			result->data[x][y] = ref->intensity[x][y] / 127;
	}

	return result;
}

ImageBN *URgbToBinary(Image *ref)
{
	ImageGS *tmp = URgbToGrayscale(ref);
	ImageBN *r = UGrayscaleToBinary(tmp);
	UFreeImageGray(tmp);
	return r;
}

Image *UBinaryToRgb(ImageBN *ref)
{
	Image *result = malloc(sizeof(Image));
	result->width = ref->width;
	result->height = ref->height;
	result->has_alpha = 1;
	result->bits_per_sample = 8;

	result->pixList = malloc(result->width * sizeof(Pixel *));
	
	for (int x = 0; x < result->width; x++)
	{
		result->pixList[x] = malloc(result->height * sizeof(Pixel));
		for (int y = 0; y < result->height; y++)
		{
			result->pixList[x][y].r = ref->data[x][y] * 255;
			result->pixList[x][y].g = ref->data[x][y] * 255;
			result->pixList[x][y].b = ref->data[x][y] * 255;
			result->pixList[x][y].a = 255;
		}
	}
	return result;
}

guchar* UGetPixelDataFromPixelsStruct(Pixel **pixList, int width, int height,
	int channel)
{
	guchar *tmp = malloc(sizeof(guchar) * width * height  * channel);
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			tmp[x*channel + y*width*channel] = pixList[x][y].r;
			tmp[x*channel + y*width*channel + 1] = pixList[x][y].g;
			tmp[x*channel + y*width*channel + 2] = pixList[x][y].b;
			
			if (channel == 4)
				tmp[x*channel + y*width*channel + 3] =
					pixList[x][y].a;
		}

	return tmp;
}

GdkPixbuf *UGetPixbufFromImage(Image *img)
{
	return gdk_pixbuf_new_from_data(
		UGetPixelDataFromPixelsStruct(img->pixList,
			img->width, img->height, img->has_alpha ? 4 : 3),
		GDK_COLORSPACE_RGB,
		img->has_alpha,
		img->bits_per_sample,
		img->width, img->height,
		(img->has_alpha ? 4 : 3) * img->width,
		NULL, NULL);
}

void URotateImage(Image *img)
{
	Pixel **tmp = malloc(sizeof(Pixel*) * img->height);

	for(int i = 0; i < img->height; i ++)
	{
		tmp[i] = malloc(sizeof(Pixel) * img->width);
		for (int j = 0; j < img->width; j ++)
			tmp[i][j] = img->pixList[img->width - j - 1][i];

	}

	img->pixList = tmp;
	
	int c = img->width;
	img->width = img->height;
	img->height = c;
}
