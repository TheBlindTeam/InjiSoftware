#include <gtk/gtk.h>

typedef struct
{
	gboolean has_clicked;
	double click_x, click_y;
} CairoNeuronData;

typedef struct
{
	GtkBuilder *builder;
	Image *img_rgb;
	CairoNeuronData *neuronData;
	NetworkSet networkSet;
} SGlobalData;
