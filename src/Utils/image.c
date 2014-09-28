#include "image.h"

Image ULoadImage(char *str)
{
	Image tmp;
	GError *error = NULL;

	tmp.pixbuf = gdk_pixbuf_new_from_file(str, &error);

	if (!tmp.pixbuf)
	{
		tmp.load_error = error;
	}

	return tmp;
}

gboolean UGetPixel(GdkPixbuf *pixbuf, Pixel *pixel, int x, int y)
{
	if (!pixbuf)
		return FALSE;

	if (x < 0 || x > gdk_pixbuf_get_width(pixbuf))
		return FALSE;

	if (y < 0 || y > gdk_pixbuf_get_height(pixbuf))
		return FALSE;

	guchar *pixelsBuf = gdk_pixbuf_get_pixels(pixbuf);
	gint channel = gdk_pixbuf_get_n_channels(pixbuf);
	gint width = gdk_pixbuf_get_width(pixbuf);

	(*pixel).r = pixelsBuf[(x*channel)+(y*width*channel)];
	(*pixel).g = pixelsBuf[(x*channel)+(y*width*channel)+1];
	(*pixel).b = pixelsBuf[(x*channel)+(y*width*channel)+2];
	if (channel == 4)
		(*pixel).a = pixelsBuf[(x*channel)+(y*width*channel)+3];

	return TRUE;
}

