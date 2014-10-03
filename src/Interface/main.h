#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

// Adrien: Initializes GTK and load Interface
void Gtk_Initialize(int argc, char *argv[]);

typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;
