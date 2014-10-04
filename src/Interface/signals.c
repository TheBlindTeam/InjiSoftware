#include "signals.h"

void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
		gtk_main_quit();
}

void on_load_button_clicked(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		GtkFileChooserDialog *dialog = user_data;
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

void file_chooser_selection_changed(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		GtkEntry *entry = user_data;
		char *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(widget));
		if (filename != NULL)
			gtk_entry_set_text(GTK_ENTRY(entry), filename);
		g_free(filename);
	}
}

void file_chooser_select_file_from_button(GtkWidget *widget,
	gpointer user_data)
{
	if (widget && user_data)
	{
		char *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(user_data));
		if (access(filename, F_OK|R_OK) != -1)
		{
			struct stat statbuf;
			if (stat(filename, &statbuf) == 0 &&
				S_ISREG(statbuf.st_mode))
			{
				gtk_widget_hide(GTK_WIDGET(user_data));
				printf("Load image file %s\n", filename);
			}
		}
		g_free(filename);
	}
}

void file_chooser_select_file(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		file_chooser_select_file_from_button(widget, widget);
	}
}

void file_chooser_cancel(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		gtk_widget_hide(GTK_WIDGET(user_data));
	}
}
