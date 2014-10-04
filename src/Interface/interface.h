#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

// Adrien: Initializes GTK and load Interface
void Gtk_Initialize(int argc, char *argv[]);
//void button_load_clicked(gpointer user_data);

typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;
