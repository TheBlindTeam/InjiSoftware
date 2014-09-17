#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "main.h"

void Gtk_Initialize (int argc, char *argv[])
{
	GtkWidget *window;

	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "window");

	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	
	gtk_widget_show(window);

	gtk_main();
}
