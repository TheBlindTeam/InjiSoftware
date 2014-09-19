#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "Interface/main.h"

int main(int argc, char *argv[])
{	
	gtk_init(&argc, &argv);
	Gtk_Initialize();
	gtk_main();

	return 0;
}
