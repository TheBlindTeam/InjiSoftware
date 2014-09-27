#include "main.h"

void Gtk_Initialize()
{
	GtkWidget *mainWindow = NULL;
	GtkBuilder *builder = NULL;
	GError *error = NULL;

	builder = gtk_builder_new();

	gtk_builder_add_from_file (
		builder,
		g_build_filename("Interface/OCR.glade", NULL),
		&error);

	if (error)
	{
		g_printerr("%s (Error Code: %d)\n", error->message, error->code);
		g_error_free (error); 
		exit(0);
	}

	mainWindow = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));

	g_signal_connect (G_OBJECT (mainWindow), "destroy",
		(GCallback)gtk_main_quit, NULL);

	gtk_widget_show_all (mainWindow);
}

