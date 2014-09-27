#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef struct gdk_image
{
	GdkPixbuf *pixbuf;
	GError *load_error;
} Image;

Image ULoadImage(char* str);

