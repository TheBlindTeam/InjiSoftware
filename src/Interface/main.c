#include "main.h"

#include <libgen.h>
#include <string.h>

void button_load_clicked()
{
//	SGlobalData *data = (SGlobalData*) user_data;
/*	gpointer data2 = data;
	data = data2;
	GtkWidget *widget2 = widget;
	widget = widget2;
*/
	printf("TEST\n");
}

void Gtk_Initialize(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *mainWindow = NULL;
	SGlobalData data;
	GError *error = NULL;

	data.builder = gtk_builder_new();

	gtk_builder_add_from_file(
		data.builder,
		g_build_filename(
			strcat((argc > 0) ? dirname(argv[0]) : "",
				"/Interface/OCR.glade"),
			NULL),
		&error);

	if (error)
	{
		g_printerr("%s (Error Code: %d)\n",
			error->message, error->code);
		g_error_free(error);
		exit(0);
	}

	mainWindow = GTK_WIDGET(gtk_builder_get_object(data.builder,
		"MainWindow"));

	/** Signals **/
	g_signal_connect(G_OBJECT(mainWindow), "destroy",
		(GCallback)gtk_main_quit, NULL);
	gtk_builder_connect_signals(data.builder, &data);


	gtk_widget_show_all(mainWindow);
}

