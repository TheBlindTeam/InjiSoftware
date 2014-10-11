#include <gtk/gtk.h>
#include "../Utils/image.h"

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
} SGlobalData;
