#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include "Interface/interface.h"

int main(int argc, char *argv[])
{
	Image img;
	if(argc > 1)
		img = ULoadImage(argv[1]);

	Gtk_Initialize(argc, argv, &img);
	gtk_main();
	return 0;
}
