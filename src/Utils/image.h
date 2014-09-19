#include <gtk/gtk.h>

typedef image
{
	unsigned int width;
	unsigned int height;
	guchar ***rgb;
	GtkImage *gtkImage;
}image;

image load(char* str);

