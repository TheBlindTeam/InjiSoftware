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

// David : Structure representing a Pixel
typedef struct gdk_pixel
{
	int r, g, b, a;
	int x, y;
} Pixel;

// Adrien: Creates an Image structure from the path str
Image ULoadImage(char* str);

// David: Get Pixel at Coordinate
gboolean UGetPixel(GdkPixbuf *pixbuf, Pixel *pixel, int x, int y);
