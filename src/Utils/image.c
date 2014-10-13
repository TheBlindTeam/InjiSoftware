#include "image.h"
#include <stdlib.h>
#include <stdio.h>

Image ULoadImage(char *str)
{
	Image tmp;
	GError *error = NULL;

	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(str, &error);

	if (!pixbuf || error)
	{
		tmp.width = 0;
		tmp.height = 0;
		tmp.bits_per_sample = 0;
		tmp.rowstride = 0;
		tmp.has_alpha = FALSE;
		tmp.pixList = NULL;
		printf("%d : %s\n", error->code, error->message);
	}
	else
	{
		tmp.width = gdk_pixbuf_get_width(pixbuf);
		tmp.height = gdk_pixbuf_get_height(pixbuf);
		tmp.bits_per_sample = gdk_pixbuf_get_bits_per_sample(pixbuf);
		tmp.rowstride = gdk_pixbuf_get_rowstride(pixbuf);
		tmp.has_alpha = gdk_pixbuf_get_has_alpha(pixbuf);
		tmp.pixList = malloc(sizeof(Pixel*) * tmp.width);
		for (int i = 0 ; i < tmp.width; i ++)
		{
			tmp.pixList[i] = malloc(sizeof(Pixel) *
					tmp.height);
			for (int j = 0; j < tmp.height; j ++)
				tmp.pixList[i][j] = UGetPixel(pixbuf, i, j);
		}
	}

	return tmp;
}

void UFreeImage(Image img)
{
	for (int i = 0; i < img.width; i ++)
		free(img.pixList[i]);
	free(img.pixList);
}

Pixel UGetPixel(GdkPixbuf *pixbuf, int x, int y)
{
	Pixel pixel;

	guchar *pixelsBuf = gdk_pixbuf_get_pixels(pixbuf);
	gint channel = gdk_pixbuf_get_n_channels(pixbuf);
	gint width = gdk_pixbuf_get_width(pixbuf);

	pixel.r = pixelsBuf[(x*channel)+(y*width*channel)];
	pixel.g = pixelsBuf[(x*channel)+(y*width*channel)+1];
	pixel.b = pixelsBuf[(x*channel)+(y*width*channel)+2];
	if (channel == 4)
		pixel.a = pixelsBuf[(x*channel)+(y*width*channel)+3];
	else
		pixel.a = 255;
	return pixel;
}

ImageGS URgbToGrayscale(Image rgbImage)
{
	ImageGS result;
	result.width = rgbImage.width;
	result.height = rgbImage.height;
	result.intensity = malloc(result.width*sizeof(guchar*));

	for(int x = 0; x < result.width; x++)
	{
		result.intensity[x] = malloc(result.height*sizeof(guchar));
	}

	for (int y = 0; y < result.height; y++)
	{
		for (int x = 0; x < result.width; x++)
		{
			result.intensity[x][y] = (
				(0.3 * rgbImage.pixList[x][y].r) +
				(0.59 * rgbImage.pixList[x][y].g) +
				(0.11 * rgbImage.pixList[x][y].b));
		}
	}
	
	return result;
}

Image UGrayscaleToRgb(ImageGS reference)
{
	Image result;
	result.width = reference.width;	
	result.height = reference.height;

	result.pixList = malloc(result.width * sizeof(Pixel*));

	for (int i = 0; i < result.width; i++)
	{
		result.pixList[i] = malloc(result.height * sizeof(Pixel));
	}

	for(int y = 0; y < result.height; y++)
	{
		for(int x = 0; x < result.width; x++)
		{
			result.pixList[x][y].r = reference.intensity[x][y];
			result.pixList[x][y].g = reference.intensity[x][y];
			result.pixList[x][y].b = reference.intensity[x][y];
			result.pixList[x][y].a = 255;
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

GdkPixbuf *UGetPixbufFromImage(Image img)
{
	return gdk_pixbuf_new_from_data(
		UGetPixelDataFromPixelsStruct(img.pixList, img.width,
			img.height, img.has_alpha ? 4 : 3),
		GDK_COLORSPACE_RGB,
		img.has_alpha,
		img.bits_per_sample,
		img.width, img.height,
		img.rowstride,
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

	img->rowstride = img->rowstride / img->height * img->width;
}
