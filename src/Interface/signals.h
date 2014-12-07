#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>


#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>
#include "unistd.h"

#include "../NeuralNetwork/neuronsUtils.h"
#include "../NeuralNetwork/gates.h"
#include "../NeuralNetwork/character.h"
#include "../PreTreatment/matrixOperation.h"
#include "../PreTreatment/segmentation.h"
#include "../PreTreatment/hough.h"
#include "../Serialization/serialization.h"
#include "../Skeleton/skeleton.h"
#include "globalData.h"

#define ZOOM_COEF 0.1

// Neuron Network Visualizer Data
#define NN_MARGIN_LEFT 50
#define NN_MARGIN_TOP 20
#define NN_SPACE_X 80
#define NN_SPACE_Y 50
#define NN_NEURON_RADIUS 12
#define NN_ARROWS_COEF 0.1

#define NN_MAXHEIGHT_COEF 75

typedef struct
{
	double x,y;
} NeuronPos;

// Associates every events (signals) to a function
void connectSignals(SGlobalData* data);

// Called when the main interface window gets closed
void on_window_destroy(GtkWidget *widget, gpointer user_data);

// Called when top menu buttons are clicked
void on_load_button_clicked(GtkWidget *widget, gpointer user_data);
void on_about_button_clicked(GtkWidget *widget, gpointer user_data);

// File loader window signals
void file_chooser_select_file_from_button(GtkWidget *widget,
	gpointer user_data);
void file_chooser_cancel(GtkWidget *widget, gpointer user_data);
void file_chooser_selection_changed(GtkWidget *widget, gpointer user_data);

// Rotation window signals
void on_rotate_img_open(GtkWidget *widget, gpointer user_data);
void on_apply_rotation(GtkWidget *widget, gpointer user_data);

// Filter window signals
void on_filter_img_open(GtkWidget *widget, gpointer user_data);
void filter_window_cancel(GtkWidget *widget, gpointer user_data);
void filter_click_apply(GtkWidget *widget, gpointer user_data);

// Network Neuron Visualizer signals
void on_load_neuron_network_visualizer(GtkWidget *widget, gpointer user_data);
void on_draw_network(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void on_click_on_network(GtkWidget *widget, GdkEventButton *event,
	gpointer user_data);
void on_click_render_network(GtkWidget *widget, gpointer user_data);
void on_click_reset(GtkWidget *widget, gpointer user_data);
void on_click_initialize(GtkWidget *widget, gpointer user_data);

// Called when the detect char button is pressed (deprecated)
void on_detect_chars(GtkWidget *widget, gpointer user_data);

// Used to draw an arrow on a GtkDrawable (NNV)
void cr_draw_arrow(cairo_t*, double, double, double, double, int);

// Segmentation, Detect Orientation & Transform Grayscale buttons clicks events
void on_click_segmentation(GtkWidget *widget, gpointer user_data);
void on_click_detect_orientation(GtkWidget *widget, gpointer user_data);
void on_click_transform_grayscale(GtkWidget *widget, gpointer user_data);
void on_click_transform_binary(GtkWidget *widget, gpointer user_data);
void on_click_transform_noiseeraser(GtkWidget *widget, gpointer user_data);
void on_click_transform_dilatation(GtkWidget *widget, gpointer user_data);
void on_click_open_training(GtkWidget *widget, gpointer user_data);
void on_click_open_learning(GtkWidget *widget, gpointer user_data);

// Called when the "zoom in" or "zoom out" button is pressed
void on_zoom_in(GtkWidget *widget, gpointer user_data);
void on_zoom_out(GtkWidget *widget, gpointer user_data);
void on_zoom_change(GtkWidget *widget, gpointer user_data);
void apply_zoom(SGlobalData *data, int change_field);

// Training
void rec_draw_box(SGlobalData *data, Box** box);
void on_click_learning_ok(GtkWidget *widget, gpointer user_data);
void on_click_learning_next(GtkWidget *widget, gpointer user_data);
void remove_first_char(SGlobalData *data);

// Learning
void on_learn_load_button_clicked(GtkWidget *widget, gpointer user_data);
void file_chooser_cancel_learning(GtkWidget *widget, gpointer user_data);
void file_chooser_selection_changed_learning(GtkWidget *widget, gpointer user_data);
void file_chooser_select_file_from_button_learn(GtkWidget *widget, gpointer user_data);
void on_click_learn_button_learn(GtkWidget *widget, gpointer user_data);
void on_click_new_network_learn(GtkWidget *widget, gpointer user_data);
void get_random_training_set(char *line);

void get_main_network(char* line);
void process_print(SGlobalData *data);
double learnRc(SGlobalData *data, int nbIter, char* fname);
void on_click_process(GtkWidget *widget, gpointer user_data);

void on_click_close_orientation(GtkWidget *widget, gpointer user_data);
