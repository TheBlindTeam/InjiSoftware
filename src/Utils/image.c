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
