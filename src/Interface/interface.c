#include "interface.h"

#include <libgen.h>
#include <string.h>

/*void button_load_clicked(GtkWidget *widget, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)user_data;
//	gpointer imageChooser = NULL;

	GtkWidget *widget2 = widget;
	widget = widget2;

	imageChooser = gtk_builder_get_object(data->builder,
		"ImageChooser");
	gtk_dialog_run(GTK_DIALOG(imageChooser));
	//gtk_widget_show(imageChooser);
//	gtk_widget_hide(imageChooser);
	gtk_main_quit();
}*/

void Gtk_Initialize(int argc, char *argv[])
{
	GtkWidget *mainWindow = NULL;
	SGlobalData data;
	gchar *uiFilename = NULL;
	GError *error = NULL;

	gtk_init(&argc, &argv);

	uiFilename = g_build_filename(
		strcat((argc > 0) ? dirname(argv[0]) : "",
		"/Interface/OCR.glade"), NULL);

	data.builder = gtk_builder_new();

	gtk_builder_add_from_file(data.builder, uiFilename, &error);
	g_free(uiFilename);

	if (error)
	{
		g_printerr("%s (Error Code: %d)\n",
			error->message, error->code);
		g_error_free(error);
		exit(0);
	}

	gtk_builder_connect_signals(data.builder, &data);

	mainWindow = GTK_WIDGET(gtk_builder_get_object(data.builder,
		"MainWindow"));


	gtk_widget_show_all(mainWindow);
}

