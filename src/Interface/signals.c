#include "signals.h"

void connectSignals(SGlobalData *data)
{
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "MainWindow")),
		"destroy",
		G_CALLBACK(on_window_destroy), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "NetworkVisualizerBtnTop")),
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
			"SegmentationBtnTop")),
		"clicked",
		G_CALLBACK(on_click_segmentation), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"Edit.Segmentation")),
		"activate",
		G_CALLBACK(on_click_segmentation), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"DetectOrientationBtnTop")),
		"clicked",
		G_CALLBACK(on_click_detect_orientation), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"View.DetectOrientation")),
		"activate",
		G_CALLBACK(on_click_detect_orientation), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"GrayscaleBtnTop")),
		"clicked",
		G_CALLBACK(on_click_transform_grayscale), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"Edit.Grayscale")),
		"activate",
		G_CALLBACK(on_click_transform_grayscale), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"BinarizeBtnTop")),
		"clicked",
		G_CALLBACK(on_click_transform_binary), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"Edit.Binarize")),
		"activate",
		G_CALLBACK(on_click_transform_binary), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"View.Training")),
		"activate",
		G_CALLBACK(on_click_open_training), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"View.Learning")),
		"activate",
		G_CALLBACK(on_click_open_learning), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"NoiseEraserBtnTop")),
		"clicked",
		G_CALLBACK(on_click_transform_noiseeraser), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"Edit.NoiseEraser")),
		"activate",
		G_CALLBACK(on_click_transform_noiseeraser), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"Edit.Dilatation")),
		"activate",
		G_CALLBACK(on_click_transform_dilatation), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LearnOKBtn")),
		"clicked",
		G_CALLBACK(on_click_learning_ok), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LearnNextBtn")),
		"clicked",
		G_CALLBACK(on_click_learning_next), data);
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
		G_OBJECT(gtk_builder_get_object(data->builder, "AboutMenu")),
		"activate",
		G_CALLBACK(on_about_button_clicked), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"SaveFileButton")),
		"clicked",
		G_CALLBACK(on_click_export_button), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "File.Export")),
		"activate",
		G_CALLBACK(on_click_export_button), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ExportCancelBtn")),
		"clicked",
		G_CALLBACK(on_click_export_cancel), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ExportExportBtn")),
		"clicked",
		G_CALLBACK(on_click_export_export), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "FCButtonOK")),
		"clicked",
		G_CALLBACK(file_chooser_select_file_from_button), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ImageChooser")),
		"selection-changed",
		G_CALLBACK(file_chooser_selection_changed), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ImageChooser")),
		"file-activated",
		G_CALLBACK(file_chooser_select_file_from_button), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"FCButtonCancel")),
		"clicked",
		G_CALLBACK(file_chooser_cancel), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"FilterCancel")),
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
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ZoomInButton")),
		"clicked",
		G_CALLBACK(on_zoom_in), data);
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ZoomOutButton")),
		"clicked",
		G_CALLBACK(on_zoom_out), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ZoomField")),
		"changed",
		G_CALLBACK(on_zoom_change), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LearnLoad")),
		"clicked",
		G_CALLBACK(on_learn_load_button_clicked), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"FCButtonCancelLearn")),
		"clicked",
		G_CALLBACK(file_chooser_cancel_learning), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LearningNetworkChooser")),
		"selection-changed",
		G_CALLBACK(file_chooser_selection_changed_learning), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder, "FCButtonOKLearn")),
		"clicked",
		G_CALLBACK(file_chooser_select_file_from_button_learn), data);
	
	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LearnButtonLearn")),
		"clicked",
		G_CALLBACK(on_click_learn_button_learn), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"LearnNew")),
		"clicked",
		G_CALLBACK(on_click_new_network_learn), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"ProcessBtnTop")),
		"clicked",
		G_CALLBACK(on_click_process), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"CloseBtnAngle")),
		"clicked",
		G_CALLBACK(on_click_close_orientation), data);

	g_signal_connect(
		G_OBJECT(gtk_builder_get_object(data->builder,
			"CloseBtnSaveOK")),
		"clicked",
		G_CALLBACK(on_click_close_save_ok), data);
}

void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
			UFreeImage(data->img_rgb);
		if (data->networkSet != NULL)
			NFreeNetworkSet(data->networkSet);
		if (data->firstBox != NULL)
			free(data->firstBox);
		if (data->segBoxArray != NULL)
			free(data->segBoxArray);
		gtk_main_quit();
	}
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
					UFreeImage(data->img_rgb);
				data->img_rgb = NULL;

				if (data->segBoxArray != NULL)
				{
					free(data->segBoxArray);
					data->segBoxArray = NULL;
					FreeBox(data->firstBox);
					data->firstBox = NULL;
					data->boxDetectIndex = 0;
					data->boxCount = 0;
				}

				gtk_entry_set_text(GTK_ENTRY(
					gtk_builder_get_object(data->builder,
						"PreviewPath")), filename);

				data->previewScale = 1;
				gtk_entry_set_text(GTK_ENTRY(
					gtk_builder_get_object(data->builder,
						"ZoomField")), "100");

				gtk_widget_hide(GTK_WIDGET(
					gtk_builder_get_object(data->builder,
						"ImageChooser")));
				data->img_rgb = ULoadImage(filename);
				if(data->tmp)
				{
					if(*data->tmp)
						free(*data->tmp);
					free(data->tmp);
					data->tmp = NULL;
				}
				if(data->pixbuf)
					g_object_unref(data->pixbuf);
				data->pixbuf = NULL;
				data->tmp = malloc(sizeof(guchar*));
				data->pixbuf = UGetPixbufFromImage(
					data->img_rgb, data->tmp);
				gtk_image_set_from_pixbuf(GTK_IMAGE(
					gtk_builder_get_object(data->builder,
						"PreviewImage")),
					data->pixbuf);
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
void file_chooser_cancel_learning(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "LearningNetworkChooser")));
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

		gtk_dialog_run(GTK_DIALOG(window));
		gtk_widget_hide(window);
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

		gint maxIter = gtk_spin_button_get_value_as_int(
			GTK_SPIN_BUTTON(gtk_builder_get_object(data->builder,
				"SBMaxIter")));

		int maxNeuron = getMaxNeuronsLayer(network);
		int maxHeight = maxNeuron * NN_MAXHEIGHT_COEF;
		int neuronSpaceY = (maxHeight - 2 * NN_NEURON_RADIUS
			* maxNeuron) / (maxNeuron - 1);

		char *paramsName[4] = {"SBLearningRate", "SBMomentum",
			"SBMaxErr", "SBOverfitCoef"};
		gdouble val[4];
		for (int i = 0; i < 4; i ++)
			val[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(
				gtk_builder_get_object(data->builder,
					paramsName[i])));

		data->networkSet->lRate = val[0];
		data->networkSet->momentum = val[1];
		data->networkSet->maxError = val[2];
		data->networkSet->overfitCoef = val[3];

		int iterCalls = 0;
		while(networkSet->learn(networkSet) && iterCalls < maxIter)
			iterCalls++;
		NComputeError(networkSet->nWork, networkSet->exSet, 0, NULL, 0);
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

		for (int i = 0; i < ((network.bias) ? network.layersSize[0]-1 :
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
			if (selectedNeuronx == lastLayer
				&& selectedNeurony == i)
				cairo_set_source_rgba(cr, 0.0, 0.5, 0.0, 1);
			else
				cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);
			cr_draw_arrow(cr, neuronPos[lastLayer][i].x,
				neuronPos[lastLayer][i].y,
				neuronPos[lastLayer][i].x + 50,
				neuronPos[lastLayer][i].y, 1);
		}

		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(data->builder,
				"ComputeErrorCB"))))
		{
			cairo_set_source_rgba(cr, 1.0, 0.5, 0.2, 1);
			cairo_move_to(cr, neuronPos[lastLayer][0].x + 120,
				100);
			data->neuronData->shouldErr = FALSE;
			int posx = 0;
			int i = 0;
			int u = 0;
			char str[1000], tmp[100];
			NComputeError(networkSet->nWork, networkSet->exSet, 1,
				str, 1000);
			while(str[i])
			{
				if (str[i] == '\n')
				{
					memcpy(tmp, str + u, i - u);
					tmp[i - u] = 0;
					u = i + 1;
					cairo_show_text(cr, tmp);
					posx += 11;
					cairo_move_to(cr, neuronPos[lastLayer]
						[0].x + 120, 100 + posx);
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
				sprintf(str, "Weight %d: %f", i+1,
					network.neurons[selectedNeuronx]
					[selectedNeurony].connectList[i]
						.weight);
				cairo_move_to(cr, neuronPos[lastLayer][0].x +
					120, 30 + 10 * i);
				cairo_show_text(cr, str);
			}
			cairo_move_to(cr, neuronPos[lastLayer][0].x + 120, 10);
			cairo_show_text(cr, str1);
			cairo_move_to(cr, neuronPos[lastLayer][0].x + 120, 20);
			cairo_show_text(cr, str2);
		}
		for (int l = 0; l < network.nbLayers; l++)
			free(neuronPos[l]);
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

		char *paramsName[7] = {"NNGate", "NNArchitecture",
			"NNTypeLearn", "NNThrIn", "NNThrOut", "NNThrHidLay",
			"NNThrBias"};
		for (int i = 0; i < 7; i ++)
			gtk_widget_set_sensitive(GTK_WIDGET(
				gtk_builder_get_object(
				data->builder, paramsName[i])), TRUE);
	}
}

void on_click_initialize(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;

		char *paramsName[7] = {"NNGate", "NNArchitecture",
			"NNTypeLearn", "NNThrIn", "NNThrOut", "NNThrHidLay",
			"NNThrBias"};
		gint val[7];
		for (int i = 0; i < 7; i ++)
		{
			GObject* object = gtk_builder_get_object(data->builder,
				paramsName[i]);
			gtk_widget_set_sensitive(GTK_WIDGET(object), TRUE);
			val[i] = gtk_combo_box_get_active(
				GTK_COMBO_BOX(object));
		}
		data->networkSet = NInitGatesNetworkSet(val[0], val[1], val[2],
			val[3], val[4], val[5], val[6]);

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
					sprintf(wname, "FilterMatrixButton%d",
						3 * x + y + 1);
					matrix[x][y] =
						gtk_spin_button_get_value(
							GTK_SPIN_BUTTON(
						gtk_builder_get_object(
							data->builder,
							wname)));
				}
			}
			Image *tmpImg = UConvolution(data->img_rgb, matrix, 3);
			
			for(int x = 0; x < 3; x++)
				free(matrix[x]);
			free(matrix);

			UFreeImage(data->img_rgb);
			data->img_rgb = tmpImg;
			if(data->tmp)
			{
				if(*data->tmp)
					free(*data->tmp);
				free(data->tmp);
				data->tmp = NULL;
			}
			if(data->pixbuf)
				g_object_unref(data->pixbuf);
			data->pixbuf = NULL;
			data->tmp = malloc(sizeof(guchar*));
			data->pixbuf = UGetPixbufFromImage(
				data->img_rgb, data->tmp);
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				data->pixbuf);
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
			Image *tmpImg = URotate(data->img_rgb, M_PI *
				(double)amount / (double)180);

			UFreeImage(data->img_rgb);

			if(data->tmp)
			{
				if(*data->tmp)
					free(*data->tmp);
				free(data->tmp);
				data->tmp = NULL;
			}
			if(data->pixbuf)
				g_object_unref(data->pixbuf);
			data->pixbuf = NULL;
			data->img_rgb = tmpImg;
			data->tmp = malloc(sizeof(guchar*));
			data->pixbuf = UGetPixbufFromImage(data->img_rgb,
				data->tmp);
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
				"PreviewImage")), data->pixbuf);

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
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			if (data->segBoxArray)
			{
				if(data->firstBox)
					FreeBox(data->firstBox);
				data->firstBox = NULL;

				for(int i = 0; i < data->boxCount; i++)
					FreeBox(data->segBoxArray[i]);
				data->segBoxArray = NULL;
			}

			ImageGS *tmpBn = URgbToGrayscale(data->img_rgb);
			ImageGS *tmpGs = MedianFilter(tmpBn, 3);
			Image *tmpImg = UGrayscaleToRgb(tmpGs);
			data->firstBox = GetBoxFromSplit(data->img_rgb, data->img_rgb);
			UFreeImageGray(tmpBn);
			UFreeImageGray(tmpGs);
			UFreeImage(tmpImg);
			
			Image *segTmpImg = DrawAllBoxes(data->img_rgb, data->firstBox, 1);
			UFreeImage(data->img_rgb);
			data->img_rgb = segTmpImg;

			if(data->tmp)
			{
				if(*data->tmp)
					free(*data->tmp);
				free(data->tmp);
				data->tmp = NULL;
			}
			if(data->pixbuf)
				g_object_unref(data->pixbuf);
			data->pixbuf = NULL;
			data->tmp = malloc(sizeof(guchar*));
			data->pixbuf = UGetPixbufFromImage(data->img_rgb, data->tmp);
			gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(data->builder, "PreviewImage")), data->pixbuf);

			apply_zoom(data, 1);
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
			ImageBN* tmpBn = URgbToBinary(data->img_rgb);
			double angle = FindInclinationAngle(tmpBn);
			UFreeImageBinary(tmpBn);

			gchar txt[20];
			sprintf(txt, "%.3f", (double)(180 * angle) / M_PI);

			GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(data->builder, "OrientationDialog"));
			gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "Angle: %s°", txt);

			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_hide(dialog);
		}
	}
}

void on_click_transform_grayscale(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			ImageGS *tmpGs = URgbToGrayscale(data->img_rgb);
			Image *tmpImg = UGrayscaleToRgb(tmpGs);
			UFreeImageGray(tmpGs);

			UFreeImage(data->img_rgb);
			data->img_rgb = tmpImg;

			if(data->tmp)
			{
				if(*data->tmp)
					free(*data->tmp);
				free(data->tmp);
				data->tmp = NULL;
			}
			if(data->pixbuf)
				g_object_unref(data->pixbuf);
			data->pixbuf = NULL;
			data->tmp = malloc(sizeof(guchar*));
			data->pixbuf = UGetPixbufFromImage(data->img_rgb,
				data->tmp);
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				data->pixbuf);

			apply_zoom(data, 1);
		}
	}
}

void on_click_transform_dilatation(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			ImageBN *tmpBn = URgbToBinary(data->img_rgb);
			ImageBN *tmpNegBN = NegativeBinaryImage(tmpBn);
			ImageBN *tmpGs = DilatationOnBinary(tmpNegBN,
				(data->img_rgb->width + data->img_rgb->height)/200);
			ImageBN *tmpNegBN2 = NegativeBinaryImage(tmpGs);
			Image *tmpImg = UBinaryToRgb(tmpNegBN2);
			UFreeImageBinary(tmpBn);
			UFreeImageBinary(tmpGs);
			UFreeImageBinary(tmpNegBN);
			UFreeImageBinary(tmpNegBN2);
			
			UFreeImage(data->img_rgb);
			data->img_rgb = tmpImg;

			if(data->tmp)
			{
				if(*data->tmp)
					free(*data->tmp);
				free(data->tmp);
				data->tmp = NULL;
			}
			if(data->pixbuf)
				g_object_unref(data->pixbuf);
			data->pixbuf = NULL;
			data->tmp = malloc(sizeof(guchar*));
			data->pixbuf = UGetPixbufFromImage(data->img_rgb,
				data->tmp);
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				data->pixbuf);

			apply_zoom(data, 1);
		}
	}
}

void on_click_transform_noiseeraser(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			ImageGS *tmpBn = URgbToGrayscale(data->img_rgb);
			ImageGS *tmpGs = MedianFilter(tmpBn, 3);
			Image *tmpImg = UGrayscaleToRgb(tmpGs);
			UFreeImageGray(tmpBn);
			UFreeImageGray(tmpGs);

			UFreeImage(data->img_rgb);
			data->img_rgb = tmpImg;

			if(data->tmp)
			{
				if(*data->tmp)
					free(*data->tmp);
				free(data->tmp);
				data->tmp = NULL;
			}
			if(data->pixbuf)
				g_object_unref(data->pixbuf);
			data->pixbuf = NULL;
			data->tmp = malloc(sizeof(guchar*));
			data->pixbuf = UGetPixbufFromImage(data->img_rgb,
				data->tmp);
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder,
					"PreviewImage")),
				data->pixbuf);

			apply_zoom(data, 1);
		}
	}
}

void on_click_transform_binary(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			ImageBN *tmpBn = URgbToBinary(data->img_rgb);
			Image *tmpImg = UBinaryToRgb(tmpBn);
			UFreeImageBinary(tmpBn);

			UFreeImage(data->img_rgb);
			data->img_rgb = tmpImg;

			if(data->tmp)
			{
				if(*data->tmp)
					free(*data->tmp);
				free(data->tmp);
				data->tmp = NULL;
			}
			if(data->pixbuf)
				g_object_unref(data->pixbuf);
			data->pixbuf = NULL;
			data->tmp = malloc(sizeof(guchar*));
			data->pixbuf = UGetPixbufFromImage(data->img_rgb, data->tmp);
			gtk_image_set_from_pixbuf(GTK_IMAGE(
				gtk_builder_get_object(data->builder, "PreviewImage")),
				data->pixbuf);

			apply_zoom(data, 1);
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
			apply_zoom(data, 1);
		}
	}
}

void on_zoom_out(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL &&
			data->previewScale > 2 *ZOOM_COEF)
		{
			data->previewScale -= ZOOM_COEF;
			apply_zoom(data, 1);
		}
	}
}

void on_zoom_change(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if (data->img_rgb != NULL)
		{
			GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(
				data->builder, "ZoomField"));
			int zoom = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
			double zm = (!zoom)?1:(double)zoom/100;
			data->previewScale = zm;
			apply_zoom(data, 0);
		}
	}
}

void apply_zoom(SGlobalData *data, int change_field)
{
	if(data->tmp)
	{
		if(*data->tmp)
			free(*data->tmp);
		free(data->tmp);
		data->tmp = NULL;
	}
	if(data->pixbuf)
		g_object_unref(data->pixbuf);
	data->pixbuf = NULL;
	data->tmp = malloc(sizeof(guchar*));
	data->pixbuf = UGetPixbufFromImage(
		data->img_rgb, data->tmp);

	GdkPixbuf *pixbuf = gdk_pixbuf_scale_simple(data->pixbuf,
		data->img_rgb->width * data->previewScale,
		data->img_rgb->height * data->previewScale, GDK_INTERP_BILINEAR);

	gtk_image_set_from_pixbuf(GTK_IMAGE(
		gtk_builder_get_object(data->builder, "PreviewImage")),
		pixbuf);
	gtk_image_set_from_pixbuf(GTK_IMAGE(
		gtk_builder_get_object(data->builder, "ImgLearning")),
		pixbuf);
	g_object_unref(pixbuf);
	if(change_field)
	{
		char str[10];
		sprintf(str, "%1.0f", data->previewScale * 100);
		gtk_entry_set_text(GTK_ENTRY(
			gtk_builder_get_object(data->builder,
				"ZoomField")), str);
	}
}
void on_click_close_orientation(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "OrientationDialog"));
		gtk_widget_hide(window);
	}
}
/*
void rec_draw_box(SGlobalData *data, Box** box)
{
	if (box->lvl != 4)
		for (int i = 0; i < box->nbSubBoxes; i++)
			rec_draw_box(data, box->subBoxes[i]);
	else
	{
		DrawBox
	}
}
*/

int get_next_char_index(Box** segBox, int from, int max)
{
	if(from >= max || from < 0)
		return -1;
	while(segBox[from]->lvl != CHARACTER && from < max)
		from++;
	if (from >= max)
		return -1;
	return from;
}

void on_click_open_training(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if(!data->img_rgb)
			return;
		GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "TrainingWindow"));
		if(data->img_rgb)
			apply_zoom(data, 1);

		data->img_bn = URgbToBinary(data->img_rgb);
		ImageGS *tmpBn = URgbToGrayscale(data->img_rgb);
		ImageGS *tmpGs = MedianFilter(tmpBn, 3);
		Image *tmpImg = UGrayscaleToRgb(tmpGs);
		data->firstBox = GetBoxFromSplit(data->img_rgb, data->img_rgb);
		UFreeImageGray(tmpBn);
		UFreeImageGray(tmpGs);
		UFreeImage(tmpImg);
		data->segBoxArray = GetBreadthBoxArray(data->firstBox, &data->boxCount);
		Image *tmp = DrawAllBoxesOfALvl(data->img_rgb, data->segBoxArray, data->boxCount, BLUE, 1, CHARACTER);
		UFreeImage(data->img_rgb);
		data->img_rgb = tmp;
		data->boxDetectIndex = get_next_char_index(data->segBoxArray, 0, data->boxCount);
		if (data->boxDetectIndex != -1)
		{
			tmp = DrawBlackPixels(data->img_rgb, data->img_bn, data->segBoxArray[data->boxDetectIndex], RED);
			UFreeImage(data->img_rgb);
			data->img_rgb = tmp;
		}
		if(data->fseg != NULL)
			free(data->fseg);
		data->fseg = NULL;
		apply_zoom(data, 0);

		gtk_dialog_run(GTK_DIALOG(window));
		gtk_widget_hide(window);
	}
}

gchar* get_text_from_txtview(SGlobalData *data)
{
	GtkTextView *view = GTK_TEXT_VIEW(gtk_builder_get_object(data->builder,
		"TxtLearning"));
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
	gchar *text;

	gtk_text_buffer_get_bounds (buffer, &start, &end);
	text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
	return text;
}

gunichar get_next_char_txtview(SGlobalData *data)
{
	GtkTextView *view = GTK_TEXT_VIEW(gtk_builder_get_object(data->builder,
		"TxtLearning"));
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
	gunichar text;

	gtk_text_buffer_get_bounds (buffer, &start, &end);
	text = gtk_text_iter_get_char(&start);

	while(text == ' ' || text == '\r' || text == '\n')
	{
		remove_first_char(data);
		gtk_text_buffer_get_bounds (buffer, &start, &end);
		text = gtk_text_iter_get_char(&start);
	}
	return text;
}

void remove_first_char(SGlobalData *data)
{
	GtkTextView *view = GTK_TEXT_VIEW(gtk_builder_get_object(data->builder,
		"TxtLearning"));

	GtkTextIter start, first;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_iter_at_offset(buffer, &first, 1);
	gtk_text_buffer_delete(buffer, &start, &first);
	gtk_text_view_set_buffer(view, buffer);
}

void on_click_learning_ok(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if(data->boxDetectIndex != -1)
		{

			GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(
				data->builder, "FileNameEntry"));
			char* filename = (char*)gtk_entry_get_text(GTK_ENTRY(entry));
			data->fseg = fopen(filename, "a");
			if(!data->fseg)
			{
				printf("Error while opening the training set file\n");
				return;
			}
			//data->segBoxArray[data->boxDetectIndex]->input = get_next_char_txtview(data);
			gunichar text = get_next_char_txtview(data);
			fprintf(data->fseg, "%c ", text);
			for(int i = 0; i < charInputSize * charInputSize; i++)
				fprintf(data->fseg, "%d", (int)data->segBoxArray[data->boxDetectIndex]->input[i]);
			fprintf(data->fseg, "\n");
			remove_first_char(data);
			data->boxDetectIndex = get_next_char_index(data->segBoxArray, data->boxDetectIndex + 1, data->boxCount);
			fclose(data->fseg);

			FILE *listF = fopen("trainingSet.list", "a+");
			char line[256];
			int isFileListed = 0;
			while(fgets(line, sizeof(line), listF))
			{
				line[strlen(line)-1] = 0;
				if(!strcmp(line, filename))
				{
					isFileListed = 1;
					break;
				}
			}
			if(!isFileListed)
				fprintf(listF, "%s\n", filename);
			fclose(listF);
		}
		if (data->boxDetectIndex != -1)
		{
			Image *tmp;
			tmp = DrawBlackPixels(data->img_rgb, data->img_bn, data->segBoxArray[data->boxDetectIndex], RED);
			UFreeImage(data->img_rgb);
			data->img_rgb = tmp;
		}
		apply_zoom(data, 0);
	}
}



void on_click_learning_next(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		data->boxDetectIndex = get_next_char_index(data->segBoxArray, data->boxDetectIndex + 1, data->boxCount);
		if (data->boxDetectIndex != -1)
		{
			Image *tmp;
			tmp = DrawBlackPixels(data->img_rgb, data->img_bn, data->segBoxArray[data->boxDetectIndex], RED);
			UFreeImage(data->img_rgb);
			data->img_rgb = tmp;
		}
		apply_zoom(data, 0);
	}
}

void on_click_open_learning(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "LearningWindow"));

		gtk_dialog_run(GTK_DIALOG(window));
		gtk_widget_hide(window);
	}
}

void on_learn_load_button_clicked(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *dialog = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "LearningNetworkChooser"));
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_hide(dialog);
	}
}

void file_chooser_selection_changed_learning(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "LearningPath"));
		char *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(widget));
		if (filename != NULL)
			gtk_entry_set_text(GTK_ENTRY(entry), filename);
		g_free(filename);
	}
}

void file_chooser_select_file_from_button_learn(GtkWidget *widget,
	gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		char *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(gtk_builder_get_object(data->builder,
				"LearningNetworkChooser")));
		if (access(filename, F_OK|R_OK) != -1)
		{
			struct stat statbuf;

			if (stat(filename, &statbuf) == 0 &&
				S_ISREG(statbuf.st_mode))
			{
				if(data->learningNet)
					NFreeNetworkSet(data->learningNet);
				data->learningNet = NULL;
				data->learningNet = NInitCharacterNetworkSet(filename);
				data->learningNet->exSet = NULL;

				GtkWidget *label = GTK_WIDGET(gtk_builder_get_object(data->builder, "FilenameLearning"));
				gtk_label_set_text(GTK_LABEL(label), basename(filename));

				gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
					data->builder, "LearnButtonLearn")), TRUE);

				gtk_widget_hide(GTK_WIDGET(
					gtk_builder_get_object(data->builder,
						"LearningNetworkChooser")));
			}
		}
		g_free(filename);
	}
}
int rand_limit(int limit)
{
	int divisor = RAND_MAX/(limit+1);
	int retval;
	do
		retval = rand() / divisor;
	while (retval > limit);
	return retval;
}

void get_random_training_set(char* line)
{
	FILE *fList = fopen("trainingSet.list", "r");
	if(fList)
	{
		int c;
		int nbLines = 0;
		while((c = getc(fList)) != EOF)
			if(c == '\n')
				nbLines++;
		rewind(fList);
		int rd = rand_limit(nbLines-1);
		while(fgets(line, 255, fList) && rd > 0)
			rd--;
		fclose(fList);
		line[strlen(line)-1] = 0;
	}
}

double learnRc(SGlobalData *data, int nbIter, char* fname)
{
	char line[256];
	get_random_training_set(line);
	if(data->learningNet->exSet)
		NFreeExempleSet(data->learningNet->exSet);
	data->learningNet->exSet = NULL;
	data->learningNet->exSet = NGetCharExempleSet(line);
	if(!data->learningNet->exSet)
		return 1;
	for(int j = 0; j < nbIter; j++)
		data->learningNet->learn(data->learningNet);
	if (rand() % 10 == 0)
	{
	for (int i = 0; i < data->learningNet->exSet->size; i ++)
	{
		if (rand() % 20 == 0)
		{
		int tmpSize = 0;
		CharOutput *c = Recognize(data->learningNet, data->learningNet->exSet->exemple[i]->input, &tmpSize);
		for (int j = 0; j < tmpSize; j ++)
		{
			int Letter = ConvertToOrderedChar(c[j].c);
			if (data->learningNet->exSet->exemple[i]->target[Letter] == 1)
				printf("\033[32;1m letter %d target %lf prob %lf\033[0m\n", Letter, data->learningNet->exSet->exemple[i]->target[Letter], c[j].prob);
			else
				printf("\033[31;1m letter %d target %lf prob %lf\033[0m\n", Letter, data->learningNet->exSet->exemple[i]->target[Letter], c[j].prob);
		}
		printf("\n\n");
		free(c);}
	}
	}
	SWrite(data->learningNet->nWork, fname);
	double r = NComputeError(data->learningNet->nWork, data->learningNet->exSet, 0, NULL, 0);
	printf("Average error : %lf learningrate %lf\n", r,data->learningNet->lRate);
	return r;
}

void on_click_learn_button_learn(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *spinner = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "SpinnerLearn"));
		gtk_spinner_start(GTK_SPINNER(spinner));
		int nbSession = gtk_spin_button_get_value(GTK_SPIN_BUTTON(
				gtk_builder_get_object(data->builder, "LearnNbSessions")));
		int nbIter = gtk_spin_button_get_value(GTK_SPIN_BUTTON(
				gtk_builder_get_object(data->builder, "LearnNbIter")));
		double learningRate = gtk_spin_button_get_value(GTK_SPIN_BUTTON(
				gtk_builder_get_object(data->builder, "LearnLR")));

		GtkWidget *entryLR = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "LearnLR"));
		data->learningNet->lRate = learningRate;

		GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(
			data->builder, "LearnFileName"));
		char *fname = (char*)gtk_entry_get_text(GTK_ENTRY(entry));
		if(nbSession > 0)
			for(int i = 0; i < nbSession; i++)
			{
				gtk_spin_button_set_value(GTK_SPIN_BUTTON(entryLR), data->learningNet->lRate);
				learnRc(data, nbIter, fname);
				data->learningNet->lRate *= 0.99;
			}
		else
			while(1)
			{
				gtk_spin_button_set_value(GTK_SPIN_BUTTON(entryLR), data->learningNet->lRate);
				learnRc(data, nbIter, fname);
				data->learningNet->lRate *= 0.99;
			}
	}
}

void on_click_new_network_learn(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		if(data->learningNet)
			NFreeNetworkSet(data->learningNet);
		data->learningNet = NULL;

		data->learningNet = NInitCharacterNetworkSet(NULL);
		GtkWidget *label = GTK_WIDGET(gtk_builder_get_object(data->builder, "FilenameLearning"));
		gtk_label_set_text(GTK_LABEL(label), "New Network");

		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(
			data->builder, "LearnButtonLearn")), TRUE);
	}
}


void on_click_export_button(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *dialog = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "ExportDialog"));
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_hide(dialog);
	}
}

void on_click_export_cancel(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *dialog = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "ExportDialog"));
		gtk_widget_hide(dialog);
	}
}

void on_click_close_save_ok(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *dialog = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "SaveDialogOK"));
		gtk_widget_hide(dialog);
	}
}

void on_click_export_export(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		GtkWidget *dialog = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "ExportDialog"));
		gtk_widget_hide(dialog);

		char *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(gtk_builder_get_object(data->builder,
				"ExportDialog")));
		GtkTextView *view = GTK_TEXT_VIEW(gtk_builder_get_object(data->builder,
			"TextView"));
		GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
		GtkTextIter start, end;
		gtk_text_buffer_get_bounds(buffer, &start, &end);
		gunichar* text = (gunichar*)gtk_text_iter_get_text(&start, &end);
		ExportTXT(filename, text);
		GtkWidget *dialogok = GTK_WIDGET(
			gtk_builder_get_object(data->builder, "SaveDialogOK"));
		gtk_dialog_run(GTK_DIALOG(dialogok));
		gtk_widget_hide(dialogok);
	}
}


void get_main_network(char* line)
{
	FILE *fList = fopen("trainingSet.list", "r");
	if(fList)
	{
		if(fgets(line, 255, fList) != NULL)
			line[strlen(line)-1] = 0;
		fclose(fList);
	}
}

void print_text(SGlobalData *data, gunichar *txt)
{
	const gchar** tmp = NULL;
/*	printf("DEBUT:%s %d a:%d\n", (gunichar*)txt, txt[0],
		g_utf8_validate((char*)txt, -1, tmp));*/
	if(g_utf8_validate((char*)txt, -1, tmp))
	{
		GtkTextView *view = GTK_TEXT_VIEW(gtk_builder_get_object(data->builder,
			"TextView"));
		GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
		GtkTextIter end;
		gtk_text_buffer_get_end_iter(buffer, &end);
		gtk_text_buffer_insert(buffer, &end, (gchar*)txt, -1);
		gtk_text_view_set_buffer(view, buffer);
	}
	else
	{
		if(txt[0] < 188 || txt[0] > 255)
			return;
		char *str[] = {"Œ", "œ", "Ÿ", "¿", "À", "Á", "Â", "Ã", "Ä",
			"Å", "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï",
			"Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "×", "Ø", "Ù", "Ú",
			"Û", "Ü", "Ý", "Þ", "ß", "à", "á", "â", "ã", "ä", "å",
			"æ", "ç", "è", "é", "ê", "ë", "ì", "í", "î", "ï", "ð",
			"ñ", "ò", "ó", "ô", "õ", "ö", "÷", "ø", "ù", "ú", "û",
			"ü", "ý", "þ", "ÿ"};
		print_text(data, (gunichar*)str[txt[0]-188]);
	}
}

void process_print(SGlobalData *data, Box* b)
{
	if (b->lvl == BLOCK)
		print_text(data, (gunichar*)"\t");
	for (int i = 0; i < b->nbSubBoxes; i ++)
		process_print(data, b->subBoxes[i]);
	if (b->lvl == BLOCK)
		print_text(data, (gunichar*)"\n\n");
	if (b->lvl == LINE)
		print_text(data, (gunichar*)"\n");
	if (b->lvl == WORD)
		print_text(data, (gunichar*)" ");
	if (b->lvl == CHARACTER)
		if (b->nbOutput >= 1 && b->output[0].prob >= 0.8)
		{
			gunichar txt[2];
			txt[0] = b->output[0].c;
			txt[1] = 0;
			print_text(data, txt);
		}
	for (int i = 0; i < b->nbSubBoxes; i ++)
		process_print(data, b->subBoxes[i]);
}

void on_click_process(GtkWidget *widget, gpointer user_data)
{
	if (widget && user_data)
	{
		SGlobalData *data = (SGlobalData*) user_data;
		ImageBN *bn = PreTreatment(data->img_rgb);
		Image *img = UBinaryToRgb(bn);
		UFreeImage(data->img_rgb);
		data->img_rgb = img;
		Box *b = Recognition(NInitCharacterNetworkSet(NULL), bn);
		img = DrawAllBoxes(data->img_rgb, b, 1);
		UFreeImage(data->img_rgb);
		data->img_rgb = NULL;
		data->img_rgb = img;
		process_print(data, b);

		apply_zoom(data, 0);
	}
}
