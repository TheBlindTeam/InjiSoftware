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

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"NNResetButton")),
		"clicked",
		G_CALLBACK(on_click_reset), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"InitButton")),
		"clicked",
		G_CALLBACK(on_click_initialize), data);
	
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"BSegmentation")),
		"clicked",
		G_CALLBACK(on_click_segmentation), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"BDetectOrientation")),
		"clicked",
		G_CALLBACK(on_click_detect_orientation), data);
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
		G_OBJECT(gtk_builder_get_object(data->builder, "Edit.Filter")),
		"activate",
		G_CALLBACK(on_filter_img_open), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LoadFileButton")),
		"clicked",
		G_CALLBACK(on_load_button_clicked), data);
	
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"AboutButton")),
		"clicked",
		G_CALLBACK(on_about_button_clicked), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "FCButtonOK")),
		"clicked",
		G_CALLBACK(file_chooser_select_file_from_button), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "ImageChooser")),
		"selection-changed",
		G_CALLBACK(file_chooser_selection_changed), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "ImageChooser")),
		"file-activated",
		G_CALLBACK(file_chooser_select_file_from_button), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"FCButtonCancel")),
		"clicked",
		G_CALLBACK(file_chooser_cancel), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "FilterCancel")),
		"clicked",
		G_CALLBACK(filter_window_cancel), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "ApplyFilter")),
		"clicked",
		G_CALLBACK(filter_click_apply), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"RotateButton")),
		"clicked",
		G_CALLBACK(on_apply_rotation), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "ZoomInButton")),
		"clicked",
		G_CALLBACK(on_zoom_in), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "ZoomOutButton")),
		"clicked",
		G_CALLBACK(on_zoom_out), data);
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

void on_about_button_clicked(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *dialog = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "AboutWindow"));
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
				if (data->img_rgb != NULL)
					UFreeImage(*data->img_rgb);
				data->previewScale = 1;
				gtk_widget_hide(GTK_WIDGET(
					gtk_builder_get_object(data->builder,
						"ImageChooser")));
				data->img_rgb = ULoadImage(filename);
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

void filter_window_cancel(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "FilterWindow")));
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

		NetworkSet *networkSet = data->networkSet;
		Network network = *networkSet->nWork;

		gint maxIter = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(
			gtk_builder_get_object(data->builder, "SBMaxIter")));

		int maxNeuron = getMaxNeuronsLayer(network);
		int maxHeight = maxNeuron * NN_MAXHEIGHT_COEF;
		int neuronSpaceY = (maxHeight - 2 * NN_NEURON_RADIUS
			* maxNeuron) / (maxNeuron - 1);

		char *paramsName[3] = {"SBLearningRate", "SBMomentum", "SBMaxErr"};
		gdouble val[3];
		for (int i = 0; i < 3; i ++)
			val[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(
				gtk_builder_get_object(data->builder, paramsName[i])));

		data->networkSet->lRate = val[0];
		data->networkSet->momentum = val[1];
		data->networkSet->maxError = val[2];

		int iterCalls = 0;
		while(networkSet->learn(networkSet) && iterCalls < maxIter)
			iterCalls++;

		cairo_set_line_width(cr, 1);
		cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);

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

		// Click detection
		int selectedNeuronx = -1;
		int selectedNeurony = -1;
		if (data->neuronData->has_clicked)
		{
			int closestLayer = 0; 
			int closestRow = 0;
			double dist = 100;
			for(int l = 0; l < network.nbLayers; l++)
				for(int n = 0; n < network.layersSize[l]; n++)
				{
					double tmpDist = sqrt(
						pow(neuronPos[l][n].x -
						data->neuronData->click_x,2) +
						pow(neuronPos[l][n].y -
						data->neuronData->click_y,2));
					if (tmpDist < dist)
					{
						dist = tmpDist;
						closestLayer = l;
						closestRow = n;
					}
				}
			if (dist < 25 && closestLayer < network.nbLayers &&
				closestRow < maxNeuron)
			{
				selectedNeuronx = closestLayer;
				selectedNeurony = closestRow;
			}
		}

		for (int l = 0; l < network.nbLayers; l++)
			for (int n = 0; n < network.layersSize[l]; n++)
			{
				int isSelect = (selectedNeuronx == l &&
					selectedNeurony == n);
				cairo_save(cr);
				if (network.bias != 0 &&
					n == network.layersSize[l] - 1 &&
					l != network.nbLayers - 1 &&
					!isSelect)
					cairo_set_source_rgba(cr, 0.7,
						0.27, 0.0, 1);
				else if (!isSelect)
					cairo_set_source_rgba(cr, 1.0,
						0.5, 0.2, 1);
				else if (isSelect)
					cairo_set_source_rgba(cr, 0.0,
						0.5, 0.0, 1);

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
						neuronPos[xCo][yCo].y, 0);
				}
				cairo_translate(cr, neuronPos[l][n].x,
					neuronPos[l][n].y);

				cairo_arc(cr, 0, 0, NN_NEURON_RADIUS,
					0, 2 * M_PI);
				cairo_fill(cr);
				cairo_stroke(cr);
				cairo_restore(cr);
			}

		for (int i = 0; i < ((network.bias) ? network.layersSize[0] - 1 :
				network.layersSize[0]); i++)
		{
			if (selectedNeuronx == 0 && selectedNeurony == i)
				cairo_set_source_rgba(cr, 0.0, 0.5, 0.0, 1);
			else
				cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);
				
			cr_draw_arrow(cr, 0, neuronPos[0][i].y,
				neuronPos[0][i].x, neuronPos[0][i].y, 0);
		}
		int lastLayer = network.nbLayers - 1;
		for (int i = 0; i < network.layersSize[lastLayer]; i++)
		{	
			if (selectedNeuronx == lastLayer && selectedNeurony == i)
				cairo_set_source_rgba(cr, 0.0, 0.5, 0.0, 1);
			else
				cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);
			cr_draw_arrow(cr, neuronPos[lastLayer][i].x,
				neuronPos[lastLayer][i].y,
				neuronPos[lastLayer][i].x + 50,
				neuronPos[lastLayer][i].y, 1);
		}

		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(data->builder, "ComputeErrorCB"))))
		{
			//cairo_move_to(cr, NN_MARGIN_LEFT - NN_NEURON_RADIUS,
			//	NN_MARGIN_TOP + maxHeight + 22);
			cairo_move_to(cr, neuronPos[lastLayer][0].x + 120, 100);
			data->neuronData->shouldErr = FALSE;
			int posx = 0;
			int i = 0;
			int u = 0;
			char str[1000], tmp[100];
			NComputeError(networkSet->nWork, networkSet->exSet, 1, str, 1000);
			while(str[i])
			{
				if (str[i] == '\n')
				{
					memcpy(tmp, str + u, i - u);
					tmp[i - u] = 0;
					u = i + 1;
					cairo_show_text(cr, tmp);
					posx += 11;
					//cairo_move_to(cr, NN_MARGIN_LEFT - NN_NEURON_RADIUS,
					//	NN_MARGIN_TOP + maxHeight + 22 + posx);
					cairo_move_to(cr, neuronPos[lastLayer][0].x + 120,
						100 + posx);
				}
				i++;
			}
		}
		if (selectedNeuronx != -1 && selectedNeurony != -1)
		{
			cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);
			char str1[10], str2[10];
			sprintf(str1, "Layer: %d", selectedNeuronx);
			sprintf(str2, "Row: %d", selectedNeurony);

			for (int i = 0; i < network.neurons[selectedNeuronx]
				[selectedNeurony].nbConnections; i++)
			{
				char str[25];
				sprintf(str, "Weight %d: %f", i+1, network.neurons
					[selectedNeuronx][selectedNeurony]
						.connectList[i].weight);
				cairo_move_to(cr, neuronPos[lastLayer][0].x + 120,
					30 + 10 * i);
				cairo_show_text(cr, str);
			}
			cairo_move_to(cr, neuronPos[lastLayer][0].x + 120, 10);
			cairo_show_text(cr, str1);
			cairo_move_to(cr, neuronPos[lastLayer][0].x + 120, 20);
			cairo_show_text(cr, str2);
		}
		free(neuronPos);
	}
}

void on_click_reset(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		data->neuronData->shouldDraw = FALSE;
		gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "NetworkDrawArea")));

		NFreeNetworkSet(data->networkSet);

		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "InitButton")), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "NNResetButton")), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "RenderButton")), FALSE);

		char *paramsName[7] = {"NNGate", "NNArchitecture", "NNTypeLearn",
			"NNThrIn", "NNThrOut", "NNThrHidLay", "NNThrBias"};
		for (int i = 0; i < 7; i ++)
			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
				data->builder, paramsName[i])), TRUE);
	}
}

void on_click_initialize(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

		char *paramsName[7] = {"NNGate", "NNArchitecture", "NNTypeLearn",
			"NNThrIn", "NNThrOut", "NNThrHidLay", "NNThrBias"};
		gint val[7];
		for (int i = 0; i < 7; i ++)
		{
			GObject* object = gtk_builder_get_object(data->builder,
				paramsName[i]);
			gtk_widget_set_sensitive(GTK_WIDGET(object), TRUE);
			val[i] = gtk_combo_box_get_active(GTK_COMBO_BOX(object));
		}
		data->networkSet = NInitNetworkSet(val[0], val[1], val[2], val[3],
			val[4], val[5], val[6]);

		// Disable buttons
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "InitButton")), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "NNResetButton")), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "RenderButton")), TRUE);
	}
}


void cr_draw_arrow(cairo_t *cr, double fromx, double fromy, double tox,
	double toy, int headArrow)
{
	cairo_save(cr);
	cairo_move_to(cr, fromx, fromy);
	cairo_line_to(cr, tox, toy);
	cairo_stroke(cr);
	// Head arrow
	if (!headArrow)
	{
		double slope = (toy-fromy)/(tox-fromx);
		double org = fromy - slope * fromx;
		tox -= (tox-fromx)/5;
		toy = slope * tox + org;
	}
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
		gint wx, wy;
		GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "NetworkDrawArea"));
		gtk_widget_translate_coordinates(widget,
			gtk_widget_get_toplevel(widget), 0, 0, &wx, &wy);

		data->neuronData->has_clicked = TRUE;
		data->neuronData->click_x = event->x - wx;
		data->neuronData->click_y = event->y - wy;
		gtk_widget_queue_draw(widget);
	}
}

void on_click_render_network(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

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

void on_filter_img_open(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

		GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "FilterWindow"));

		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_hide(dialog);
	}
}

void filter_click_apply(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			double **matrix = malloc(3 * sizeof(double*));
			for(int x = 0; x < 3; x++)
			{
				matrix[x] = malloc(3 * sizeof(double));
				for(int y = 0; y < 3; y++)
				{
					char wname[25];
					sprintf(wname, "FilterMatrixButton%d", 3*x+y+1);
					matrix[x][y] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(
						gtk_builder_get_object(data->builder, wname)));
				}
			}
			Image tmpImg = UConvolution(*data->img_rgb, matrix, 3);
			UFreeImage(*data->img_rgb);
			data->img_rgb = &tmpImg;
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				UGetPixbufFromImage(*data->img_rgb));
		}
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

			GdkPixbuf *pixbuf = UGetPixbufFromImage(*data->img_rgb);
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder, "PreviewImage")),
				pixbuf);
			g_object_unref(pixbuf);
			
			GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(
				data->builder, "RotationSelect"));

			gtk_widget_hide(dialog);
		}
		
	}
}

void on_click_segmentation(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		printf("click_segmentation 1\n");
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			if (data->segBoxArray == NULL)
			{
				int count;
				Box box = GetBoxFromSplit(*data->img_rgb);
				data->segBoxArray = GetBreadthBoxArray(box, &count);
				gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(
					data->builder, "BSegmentation")), "Detect");
				data->boxCount = count;
				return;
			}
			if (data->boxDetectIndex == data->boxCount)
				return;
			printf("test");
/*			for (int i = 0; i < data->boxDetectIndex; i++)
			{
				DrawNotInSubBoxes(data->img_rgb, boxArray[i],
					(i+1 == data->boxDetectIndex) ? BLUE : RED)
			}*/
			if (data->boxDetectIndex != 0)
				DrawNotInSubBoxes(*data->img_rgb,
					data->segBoxArray[data->boxDetectIndex - 1], BLUE);
			DrawNotInSubBoxes(*data->img_rgb,
				data->segBoxArray[data->boxDetectIndex], RED);
			data->boxDetectIndex++;
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder, "PreviewImage")),
				UGetPixbufFromImage(*data->img_rgb));
		printf("click_segmentation 2\n");
		}
	}
}

void on_click_detect_orientation(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			double angle = FindInclinationAngle(
				UGrayscaleToBinary(URgbToGrayscale(*data->img_rgb)));

			gchar txt[20];
			sprintf(txt, "%f", angle);
			gtk_entry_set_text(GTK_ENTRY(
				gtk_builder_get_object(data->builder, "DetectAngleVal")), txt);
/*
			Image tmpImg = URotate(*data->img_rgb, angle);

			UFreeImage(*data->img_rgb);

			data->img_rgb = &tmpImg;

			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder, "PreviewImage")),
				UGetPixbufFromImage(*data->img_rgb));*/
		}
	}
}
void on_zoom_in(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			data->previewScale += ZOOM_COEF;
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				gdk_pixbuf_scale_simple(UGetPixbufFromImage(*data->img_rgb),
					data->img_rgb->width * data->previewScale,
					data->img_rgb->height * data->previewScale,
					GDK_INTERP_BILINEAR));
		}
	}
}

void on_zoom_out(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL && data->previewScale > 2 * ZOOM_COEF)
		{
			data->previewScale -= ZOOM_COEF;
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				gdk_pixbuf_scale_simple(UGetPixbufFromImage(*data->img_rgb),
					data->img_rgb->width * data->previewScale,
					data->img_rgb->height * data->previewScale,
					GDK_INTERP_BILINEAR));
		}
	}
}
