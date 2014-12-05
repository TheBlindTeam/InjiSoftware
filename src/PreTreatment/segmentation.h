#ifndef SPLIT_H
#define SPLIT_H
#include "../Utils/image.h"
#include <math.h>

extern const Pixel BoxColor[];

typedef struct
{
	int nb_chars;
	gchar *c;
	double prob;
}CharOutput;

typedef enum
{
	NOTEXT = 0,
	BLOCK = 1,
	LINE = 2,
	WORD = 3,
	CHARACTER = 4,
	MAIN = 5
}SegmentationLevel;

typedef struct
{
	int x1, y1, x2, y2;
} Rectangle;

typedef struct Box
{
	Rectangle rectangle;
	SegmentationLevel lvl;
	int nbOutput;
	CharOutput *output;
	ImageBN *charImg;
	double *input;
	int capacity;
	int nbSubBoxes;
	struct Box **subBoxes;
} Box;

typedef enum
{
	HORIZONTAL,
	VERTICAL
} Orientation;

typedef struct BoxElementList
{
	Box *value;
	struct BoxElementList *next;
} BoxElementList;

typedef BoxElementList *BoxList;

//Adds an element box b in a list list and returns it.
BoxList AddInList(BoxList list, Box *b);

//Returns the number of elements in the list list.
int LengthBoxList(BoxList list);

//Converts the list to an array.
Box **BoxListToArray(BoxList list, int *count);

//frees the list list.
void FreeBoxList(BoxList list);

Box *InitBox();

void AddInSubBoxes(Box *b, Box *toBeAdded);

//Frees the box b.
void FreeBox(Box *b);

/*Changes the iterators used in the split and cross functions according to the orientation of the crossing.*/
void GetIterPrim(Orientation orient, int *primWidth, int *primHeight);
 //idem for the second coefficient
void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight);

//Returns if a whole line is blank
int isBlank(ImageBN *img, Box *b, Orientation orient, int start, int SpaceColor);
/**/

int *GetSpaceArray(ImageBN *img, Box *b, Orientation orient, int *size);

int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r);

int SpacesVariance(int *spaces, int nbSpaces, int add, double *r);

int ClassifySpace(int *spaces, int nbSpaces, int *r, double *min);

int Split(ImageBN *img, Box *b, Box *parent, Orientation orient, int minSpace, int spaceColor);

/*Removes the margins froms the image*/
void CutMargin(ImageBN *img, Box *b, int V, int H, int spaceColor);

void GetCharsFromImage(ImageBN *img, Box *b);

void GetWordsFromImage(ImageBN *img, Box *b);

void GetLinesFromImage(ImageBN *img, Box *b);

void GetBlocksFromImage(ImageBN *img, ImageBN *mask, Box *b);
/**/
Box *GetBoxFromSplit(Image *img);

/**/
Box **GetBreadthBoxArray(Box *b, int *count);

/**/
void GetBreadthBoxArrayAux(BoxList list);

/*Draws coloured rectangles in all spaces that are not text*/
Image *DrawNotInSubBoxes(Image *img, Box *b, Pixel p);

Image *DrawBox(Image *img, Box *b, Pixel p, int thickness);

Image *DrawWhitePixels(Image *img, ImageBN *mask, Box *b, Pixel p);

Image *DrawBlackPixels(Image *img, ImageBN *mask, Box *b, Pixel p);

ImageBN *DilatationOnBinary(ImageBN *img, int coef);
#endif
