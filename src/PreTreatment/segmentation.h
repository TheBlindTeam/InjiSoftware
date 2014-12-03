#ifndef SPLIT_H
#define SPLIT_H
#include "../Utils/image.h"
#include <math.h>

typedef enum
{
	NOTEXT = 0,
	BLOCK = 1,
	LINE = 2,
	WORD = 3,
	CHARACTER = 4
}SegmentationLevel;

typedef struct
{
	int x1, y1, x2, y2;
} Rectangle;

typedef struct Box
{
	Rectangle rectangle;
	SegmentationLevel lvl;
	int nbSubBoxes;
	struct Box **subBoxes;
} Box;

typedef struct BoxElementList
{
	Box *value;
	struct BoxElementList *next;
} BoxElementList;

typedef BoxElementList *BoxList;

typedef enum
{
	HORIZONTAL,
	VERTICAL
} Orientation;

//Adds an element box b in a list list and returns it.
BoxList AddInList(BoxList list, Box *b);

//Returns the number of elements in the list list.
int LengthBoxList(BoxList list);

//Converts the list to an array.
Box **BoxListToArray(BoxList list, int *count);

//frees the list list.
void FreeBoxList(BoxList list);

//Frees the box b.
void FreeBox(Box *b);

/*Changes the iterators used in the split and cross functions according to the orientation of the crossing.*/
void GetIterPrim(Orientation orient, int *primWidth, int *primHeight);
 //idem for the second coefficient
void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight);

//Returns if a whole line is blank
int isBlank(ImageBN *img, Box *b, Orientation orient, int start);
/**/
BoxList Split(ImageBN *img, Box *b, Orientation orient, int minBlank);

/*Removes the margins froms the image*/
void CutMargin(ImageBN *img, Box *b, int V, int H);

/**/
Box *GetBoxFromSplit(Image *img);

/**/
Box **GetBreadthBoxArray(Box *b, int *count);

/**/
void GetBreadthBoxArrayAux(BoxList list);

/*Draws coloured rectangles in all spaces that are not text*/
Image *DrawNotInSubBoxes(Image *img, Box *b, Pixel p);

Image *DrawBoxes(Image *img, Box *b, Pixel p);
#endif
