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
		tmp.pixList = NULL;
		printf("%d : %s\n", error->code, error->message);
	}
	else
	{
		tmp.width = gdk_pixbuf_get_width(pixbuf);
		tmp.height = gdk_pixbuf_get_height(pixbuf);
		tmp.pixList = (Pixel**)malloc(sizeof(Pixel*) * tmp.width);
		for (int i = 0 ; i < tmp.width; i ++)
		{
			tmp.pixList[i] = (Pixel*)malloc(sizeof(Pixel) *
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

