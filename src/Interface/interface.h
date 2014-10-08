#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../Utils/image.h"

// Adrien: Initializes GTK and load Interface
void Gtk_Initialize(int argc, char *argv[], Image *img);

typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
	Image *img_rgb;
} SGlobalData;
