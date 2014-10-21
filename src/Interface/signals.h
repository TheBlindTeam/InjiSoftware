#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/stat.h>
#include "unistd.h"

#include "../NeuralNetwork/neuronsUtils.h"
#include "../PreTreatment/matrixOperation.h"
#include "globalData.h"

// Neuron Network Visualizer Data
#define NN_MARGIN_LEFT 20
#define NN_MARGIN_TOP 20
#define NN_SPACE_X 50
#define NN_SPACE_Y 50
#define NN_NEURON_RADIUS 12

#define NN_MAXHEIGHT_COEF 75

typedef struct
{
	double x,y;
} NeuronPos;

void connectSignals(SGlobalData* data);

void on_window_destroy(GtkWidget *widget, gpointer user_data);

void on_load_button_clicked(GtkWidget *widget, gpointer user_data);
void file_chooser_select_file_from_button(GtkWidget *widget,
	gpointer user_data);
void file_chooser_cancel(GtkWidget *widget, gpointer user_data);

void on_rotate_img_open(GtkWidget *widget, gpointer user_data);
void on_apply_rotation(GtkWidget *widget, gpointer user_data);

void on_load_neuron_network_visualizer(GtkWidget *widget, gpointer user_data);
void on_draw_network(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void on_click_on_network(GtkWidget *widget, GdkEventButton *event,
	gpointer user_data);

void on_detect_chars(GtkWidget *widget, gpointer user_data);
