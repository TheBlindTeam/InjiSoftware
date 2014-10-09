#include "interface.h"

#include <libgen.h>
#include <string.h>


void Gtk_Initialize(int argc, char *argv[], Image *img)
{
	GtkWidget *mainWindow = NULL;
	SGlobalData *data = malloc(sizeof(SGlobalData));
	gchar *uiFilename = NULL;
	GError *error = NULL;

	gtk_init(&argc, &argv);

	uiFilename = g_build_filename(
		strcat((argc > 0) ? dirname(argv[0]) : "",
		"/Interface/OCR.glade"), NULL);

	data->img_rgb = img;
	data->builder = gtk_builder_new();

	gtk_builder_add_from_file(data->builder, uiFilename, &error);
	g_free(uiFilename);

	if (error)
	{
		g_printerr("%s (Error Code: %d)\n",
			error->message, error->code);
		g_error_free(error);
		exit(0);
	}

	connectSignals(data);

	mainWindow = GTK_WIDGET(gtk_builder_get_object(data->builder,
		"MainWindow"));

	gtk_widget_show_all(mainWindow);
}
