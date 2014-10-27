#include <gtk/gtk.h>

// Struct holding informations about the Neural Network Visualizer
typedef struct
{
	gboolean has_clicked, shouldDraw, shouldErr;
	double click_x, click_y;
} CairoNeuronData;

// GlobalData holds informations which are passed through events
typedef struct
{
	GtkBuilder *builder;
	Image *img_rgb;
	CairoNeuronData *neuronData;
	NetworkSet* networkSet;
	Box** segBoxArray;
	Box* firstBox;
	int boxDetectIndex, boxCount;
	double previewScale;
} SGlobalData;
