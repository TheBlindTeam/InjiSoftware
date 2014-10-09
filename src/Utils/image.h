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

// Adrien : Base structure of the image to be manipulated
typedef struct gdk_image
{
	int width, height, bits_per_sample, rowstride;
	gboolean has_alpha;
	Pixel **pixList;
} Image;

// David : Base structure of a GrayScale image
typedef struct
{
	int width, height;
	guchar** intensity;
} ImageGS;


// Adrien: Creates an Image structure from the path str
Image ULoadImage(char* str);

// Laurent : Free the Pixels buffer of an Image
void UFreeImage(Image img);

// David : Get Pixel at Coordinate
Pixel UGetPixel(GdkPixbuf *pixbuf, int x, int y);

// David : Translate RGB Image to GrayScale Image
ImageGS URgbToGrayscale(Image rgbImage);

// Adrien: Get packed pixels data from Pixels structure
guchar* UGetPixelDataFromPixelsStruct(Pixel **pixlist, int width,
	int height, int channel);

// Adrien: Create pixbuf from Pixel struct
GdkPixbuf *UGetPixbufFromImage(Image image);
