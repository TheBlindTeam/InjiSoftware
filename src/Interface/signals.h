#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/stat.h>
#include "unistd.h"

#include "globalData.h"

void connectSignals(SGlobalData* data);

void on_window_destroy(GtkWidget *widget, gpointer user_data);

void on_load_button_clicked(GtkWidget *widget, gpointer user_data);
void file_chooser_select_file_from_button(GtkWidget *widget,
	gpointer user_data);
void file_chooser_cancel(GtkWidget *widget, gpointer user_data);

void on_rotate_img_left(GtkWidget *widget, gpointer user_data);
