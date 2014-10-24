#include <gtk/gtk.h>

typedef struct
{
	gboolean has_clicked, shouldDraw, shouldBlock, shouldErr;
	double click_x, click_y;
} CairoNeuronData;

typedef struct
{
	GtkBuilder *builder;
	Image *img_rgb;
	CairoNeuronData *neuronData;
	NetworkSet networkSet;
	Box* segBoxArray;
	int boxDetectIndex, boxCount;
} SGlobalData;
