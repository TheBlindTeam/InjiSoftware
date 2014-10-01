#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <glib/gstdio.h>

#include "Interface/main.h"
#include "Utils/image.h"

int main(int argc, char *argv[])
{
	Image img;
	img = ULoadImage("test.jpg");

	if (img.pixList)
	{
		g_printf("Img Size: %dx%d\n",
			img.width,
			img.height);
	}
	else
		g_printf("ERRRRROOOOR");


	Gtk_Initialize(argc, argv);
	gtk_main();
	return 0;
}
