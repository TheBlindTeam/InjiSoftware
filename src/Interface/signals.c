#include "signals.h"

void connectSignals(SGlobalData *data)
{
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "MainWindow")),
		"destroy",
		G_CALLBACK(on_window_destroy), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "TestBut1")),
		"clicked",
		G_CALLBACK(on_load_neuron_network_visualizer), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "TestBut2")),
		"clicked",
		G_CALLBACK(on_detect_chars), data);
	
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"NetworkDrawArea")),
		"draw",
		G_CALLBACK(on_draw_network), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"NetworkVisualizer")),
		"button-press-event",
		G_CALLBACK(on_click_on_network), data);

	/* Menu */

	// File
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "File.New")),
		"activate",
		G_CALLBACK(on_load_button_clicked), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "File.Quit")),
		"activate",
		G_CALLBACK(on_window_destroy), data);
	// Edit
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"Edit.RotateImg")),
		"activate",
		G_CALLBACK(on_rotate_img_open), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LoadFileButton")),
		"clicked",
		G_CALLBACK(on_load_button_clicked), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "FCButtonOK")),
		"clicked",
		G_CALLBACK(file_chooser_select_file_from_button), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"FCButtonCancel")),
		"clicked",
		G_CALLBACK(file_chooser_cancel), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"RotateButton")),
		"clicked",
		G_CALLBACK(on_apply_rotation), data);
}

void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
		gtk_main_quit();
}

void on_load_button_clicked(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *dialog = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "ImageChooser"));
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_hide(dialog);
	}
}

void file_chooser_selection_changed(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "ImagePath"));
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
		SGlobalData *data = (SGlobalData*) user_data;
		char *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(gtk_builder_get_object(data->builder,
				"ImageChooser")));
		if (access(filename, F_OK|R_OK) != -1)
		{
			struct stat statbuf;

			if (stat(filename, &statbuf) == 0 &&
				S_ISREG(statbuf.st_mode))
			{
				gtk_widget_hide(GTK_WIDGET(
					gtk_builder_get_object(data->builder,
						"ImageChooser")));

				*data->img_rgb = ULoadImage(filename);

				gtk_image_set_from_pixbuf(GTK_IMAGE(
					gtk_builder_get_object(data->builder,
						"PreviewImage")),
					UGetPixbufFromImage(*data->img_rgb));
			}
		}
		g_free(filename);
	}
}

void file_chooser_cancel(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "ImageChooser")));
	}
}

void on_load_neuron_network_visualizer(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "NetworkVisualizer"));

		gtk_widget_show_all(window);
	}
}

void on_draw_network(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	if (widget && cr && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

		NetworkSet networkSet = data->networkSet;

		Network network =  *networkSet.nWork;

		while(networkSet.learn(&networkSet))
		{
			
		}


		cairo_set_line_width(cr, 1);
		cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);


		if (data->neuronData->has_clicked)
		{}


		for(int l = 0; l < network.nbLayers; l++)
		{
			for(int n = 0; n < network.layersSize[l]; n++)
			{
				printf("%d %d %d\n", l, n, network.layersSize[l]);
				cairo_translate(cr, 20 + l * 50, 20 + n * 25);
				cairo_arc(cr, 0, 0, 10, 0, 2 * M_PI);
				cairo_stroke(cr);
				cairo_fill(cr);
			}
		}


/*		cairo_translate(cr, data->neuronData->click_x,
			data->neuronData->click_y);
		cairo_arc(cr, 0, 0, 10, 0, 2 * M_PI);
		cairo_stroke(cr);

		cairo_fill(cr);
*/
/*		cairo_set_source_rgba(cr, 0.7, 0.5, 0.8, 1);
		cairo_set_line_width(cr, 0.5);
		cairo_move_to(cr, 0,0);//data->neuronData->click_x,
			//data->neuronData->click_y);
		cairo_line_to(cr, 100, 100);
		cairo_stroke(cr);*/
	}
}

void on_click_on_network(GtkWidget *widget, GdkEventButton *event,
	gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

		data->neuronData->has_clicked = TRUE;
		data->neuronData->click_x = event->x;
		data->neuronData->click_y = event->y;
		gtk_widget_queue_draw(widget);
	}
}

void on_rotate_img_open(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

		GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "RotationSelect"));

		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_hide(dialog);
	}
}

void on_apply_rotation(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "RotationAmount"));
		int amount = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));

		if (amount != 0)
		{
			Image tmpImg = URotate(*data->img_rgb, amount);

			UFreeImage(*data->img_rgb);

			data->img_rgb = &tmpImg;

			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				UGetPixbufFromImage(*data->img_rgb));
			
			GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(
				data->builder, "RotationSelect"));

			gtk_widget_hide(dialog);
		}
		
	}
}

void on_detect_chars(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
	//	SGlobalData *data = (SGlobalData*) user_data;

	}
}
