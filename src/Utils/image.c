#include "image.h"

Image ULoadImage(char *str)
{
	Image tmp;

	GdkPixbuf pixbuf = gdk_pixbuf_new_from_file(str, &error);

	if (!pixbuf)
	{
		tmp.width = 0;
		tmp.height = 0;
		tmp.pixList = NULL;
	}
	else
	{
		tmp.width = gdk_pixbuf_get_width(pixbuf);
		tmp.height = gdk_pixbuf_get_height(pixbuf);
		tmp.pixList = (Pixel**)malloc(sizeof(Pixel*) * tmp.height)
		for (int i = 0 ; i < tmp.height; i ++)
		{
			tmp.pixList[i] = (Pixel*)malloc(sizeof(Pixel) * tmp.width);
			for (int j = 0; j < tmp.width; j ++)
				tmp.pixList[i][j] = UGetPixel(&pixbuf, i, j);
		}
	}
	return tmp;
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

