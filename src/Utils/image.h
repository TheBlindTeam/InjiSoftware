#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/*
	Image Utils
*/

// Adrien: Base structure of the image to be manipulated
typedef struct gdk_image
{
	GdkPixbuf *pixbuf;
	GError *load_error;
} Image;

// Adrien: Creates an Image structure from the path str
Image ULoadImage(char* str);
