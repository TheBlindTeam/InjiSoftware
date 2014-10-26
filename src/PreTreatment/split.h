#ifndef SPLIT_H
#define SPLIT_H
#include "../Utils/image.h"
#include <math.h>
extern const guchar BLACKGS;

typedef struct
{
	int x1, y1, x2, y2;
} Rectangle;

typedef struct Box
{
	Rectangle rectangle;
	int nbSubBoxes;
	struct Box * subBoxes;
} Box;

typedef struct BoxElementList
{
	Box value;
	struct BoxElementList *next;
} BoxElementList;

typedef BoxElementList *BoxList;

typedef enum
{
	HORIZONTAL,
	VERTICAL
} Orientation;

BoxList AddInList(BoxList list, Box b);

int LengthBoxList(BoxList list);

Box *BoxListToArray(BoxList list, int *count);

void FreeBoxList(BoxList list);

//Interessant
void GetIterPrim(Orientation orient, int *primWidth, int *primHeight);

void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight);

int isBlank(ImageBN *img, Box b, Orientation orient, int start);

int *GetSpaceArray(ImageBN *img, Box b, Orientation orient, int *size);

int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r);

int SpacesVariance(int *spaces, int nbSpaces, int add, double *r);

//Interessant
int ClassifySpace(int *spaces, int nbSpaces, int *r, double *min);


//Interessant
void Split(ImageBN *img, Box *b, Orientation orient, int minBlank);

void CutMargin(ImageBN *img, Box *b);

void DetectSplitOrientation(ImageBN *img, Box *b);

Box GetBoxFromSplit(Image *img);

Box *GetBreadthBoxArray(Box b, int *count);

void GetBreadthBoxArrayAux(BoxList list);

void DrawNotInSubBoxes(Image *img, Box b, Pixel p);
#endif
