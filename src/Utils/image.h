#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/*
	Image Utils
*/

// David : Structure representing a Pixel
typedef struct gdk_pixel
{
	guchar r, g, b, a;
} Pixel;

// Adrien: Base structure of the image to be manipulated
typedef struct gdk_image
{
	int width, height;
	Pixel **pixList;
} Image;


// Adrien: Creates an Image structure from the path str
Image ULoadImage(char* str);

// David: Get Pixel at Coordinate
Pixel UGetPixel(GdkPixbuf *pixbuf, int x, int y);
