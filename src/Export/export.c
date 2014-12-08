#include "export.h"

void ExportTXT(char* filename, gunichar* text)
{
	FILE *f = fopen(filename, "w");
	if(f)
	{
		fprintf(f, "%s", (gchar*)text);
		fclose(f);
	}
}
