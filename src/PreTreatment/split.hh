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

/*Changes the iterators used in the split and cross functions according to the
 orientation of the crossing.*/
void GetIterPrim(Orientation orient, int *primWidth, int *primHeight);
 //idem for the second coefficient
void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight);

//Returns if a whole line is blank
int isBlank(ImageBN *img, Box *b, Orientation orient, int start);

/*Returns an array with indexes corresponding to all the possible sizes of
 spaces in the image (from 0 to the size of the image).
The value at each index is the number of spaces of size = index*/
int *GetSpaceArray(ImageBN *img, Box *b, Orientation orient, int *size);

/*Gets the expected value of the spaces*/
int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r);

/*Gets the variance of the spaces*/
int SpacesVariance(int *spaces, int nbSpaces, int add, double *r);

/**/
int ClassifySpace(int *spaces, int nbSpaces, int *r, double *min);

/**/
void Split(ImageBN *img, Box *b, Orientation orient, int minBlank, int state);

/*Removes the margins froms the image*/
void CutMargin(ImageBN *img, Box *b, int V, int H);

/**/
void DetectSplitOrientation(ImageBN *img, Box *b);

/**/
Box *GetBoxFromSplit(Image *img);

/**/
Box **GetBreadthBoxArray(Box *b, int *count);

/**/
void GetBreadthBoxArrayAux(BoxList list);

/*Draws coloured rectangles in all spaces that are not text*/
void DrawNotInSubBoxes(Image *img, Box *b, Pixel p);
#endif
