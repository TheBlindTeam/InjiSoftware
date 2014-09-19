#include "main.h"

void print_hello ()
{
        g_print ("Hello World\n");
}


void Gtk_Initialize()
{
	GtkBuilder *builder;
	GObject *window;
	GObject *button;

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "Interface/builder.ui", NULL);

	window = gtk_builder_get_object (builder, "window");
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	button = gtk_builder_get_object (builder, "button1");
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

	button = gtk_builder_get_object (builder, "button2");
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

	button = gtk_builder_get_object (builder, "quit");
	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
}

