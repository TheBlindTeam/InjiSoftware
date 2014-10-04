#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "interface.h"

void on_window_destroy(GtkWidget *widget, gpointer user_data);

void on_load_button_clicked(GtkWidget *widget, gpointer user_data);
void file_chooser_select_file_from_button(GtkWidget *widget,
	gpointer user_data);
void file_chooser_select_file(GtkWidget *widget, gpointer user_data);
