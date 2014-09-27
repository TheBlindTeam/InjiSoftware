#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef struct gdk_image
{
	GdkPixbuf *pixbuf;
	GError *load_error;
} Image;
//Shelby : The structure used for manupulating the images

Image ULoadImage(char* str);
//Shelby : It loads the image at the path str and it returns this image
