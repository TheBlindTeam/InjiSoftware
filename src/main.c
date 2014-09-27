#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <glib/gstdio.h>

#include "Interface/main.h"
#include "Utils/image.h"

int main(int argc, char *argv[])
{
	Image img;
	img = ULoadImage("./test.jpg");

	if (img.pixbuf)
	{
		g_printf("Img Size: %dx%d",
			gdk_pixbuf_get_width(img.pixbuf),
			gdk_pixbuf_get_height(img.pixbuf));
	}
	else
		g_printf("%s", (*img.load_error).message);

	Gtk_Initialize(argc, argv);
	gtk_main();
	return 0;
}
