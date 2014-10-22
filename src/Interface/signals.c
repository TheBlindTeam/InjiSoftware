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

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"RenderButton")),
		"clicked",
		G_CALLBACK(on_click_render_network), data);
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

		if (!data->neuronData->shouldDraw)
			return;

		NetworkSet networkSet = data->networkSet;
		Network network =  *networkSet.nWork;

		int maxNeuron = getMaxNeuronsLayer(network);
		int maxHeight = maxNeuron * NN_MAXHEIGHT_COEF;
		int neuronSpaceY = (maxHeight - 2 * NN_NEURON_RADIUS
			* maxNeuron) / (maxNeuron - 1);
		
		while(networkSet.learn(&networkSet) && 
			!data->neuronData->shouldBlock);


		cairo_set_line_width(cr, 1);
		cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);


		if (data->neuronData->has_clicked)
		{}

		NeuronPos** neuronPos = malloc(network.nbLayers *
			sizeof(NeuronPos *));

		for (int l = 0; l < network.nbLayers; l++)
		{
			neuronPos[l] = malloc(network.layersSize[l] *
				sizeof(NeuronPos));
			for (int n = 0; n < network.layersSize[l]; n++)
			{
				neuronPos[l][n].x = NN_MARGIN_LEFT +
					l * NN_SPACE_X;
				neuronPos[l][n].y = NN_NEURON_RADIUS +
					NN_MARGIN_TOP + (maxHeight -
						network.layersSize[l] * 2
						* NN_NEURON_RADIUS -
						(network.layersSize[l] - 1) *
						neuronSpaceY) / 2 +
						2 * NN_NEURON_RADIUS * n +
						n * neuronSpaceY;
			}
		}
		for (int l = 0; l < network.nbLayers; l++)
			for (int n = 0; n < network.layersSize[l]; n++)
			{
				cairo_save(cr);
				if (network.bias != 0 &&
					n == network.layersSize[l] - 1 &&
					l != network.nbLayers - 1)
					cairo_set_source_rgba(cr, 0.7, 0.27,
						0.0, 1);
				else
					cairo_set_source_rgba(cr, 1.0, 0.5,
						0.2, 1);

				for (int c = 0; c < network.neurons[l][n].
						nbConnections; c++)
				{
					int xCo = network.neurons[l][n].
						connectList[c].layer;
					int yCo = network.neurons[l][n].
						connectList[c].index;
					cr_draw_arrow(cr, neuronPos[l][n].x,
						neuronPos[l][n].y,
						neuronPos[xCo][yCo].x,
						neuronPos[xCo][yCo].y);
				}
				cairo_translate(cr, neuronPos[l][n].x,
					neuronPos[l][n].y);

				cairo_arc(cr, 0, 0, NN_NEURON_RADIUS,
					0, 2 * M_PI);
				cairo_fill(cr);
				cairo_stroke(cr);
				cairo_restore(cr);

				cairo_move_to(cr, NN_MARGIN_LEFT -
						NN_NEURON_RADIUS,
					NN_MARGIN_TOP+maxHeight+30);
				cairo_show_text(cr, "Errors: None");

			}

		free(neuronPos);
	}
}

void cr_draw_arrow(cairo_t *cr, double fromx, double fromy, double tox,
	double toy)
{
	cairo_save(cr);
	cairo_move_to(cr, fromx, fromy);
	cairo_line_to(cr, tox, toy);
	cairo_stroke(cr);
	// Head arrowi
	double slope = (toy-fromy)/(tox-fromx);
	double org = fromy - slope * fromx;
	tox -= (tox-fromx)/5;
	toy = slope * tox + org;
	double length = 10;
	double angle = atan2(toy - fromy, tox - fromx) + M_PI;
	cairo_move_to(cr, tox, toy);
	cairo_line_to(cr, tox + length * cos(angle - 0.5),
		toy + length * sin(angle - 0.5));
	cairo_line_to(cr, tox + length * cos(angle + 0.5),
		toy + length * sin(angle + 0.5));
	cairo_line_to(cr, tox, toy);
	cairo_fill(cr);
	cairo_stroke(cr);
	cairo_restore(cr);
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

void on_click_render_network(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

		GtkWidget *cb = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "BlockStepsCheckbox"));

		data->neuronData->shouldBlock = gtk_toggle_button_get_active(
			GTK_TOGGLE_BUTTON(cb));
		data->neuronData->shouldDraw = TRUE;
		gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "NetworkDrawArea")));
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
