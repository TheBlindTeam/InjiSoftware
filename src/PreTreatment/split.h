#include "../Utils/image.h"
typedef struct
{
	int x, y, width, height;
} Rectangle;

typedef struct Box
{
	int nbSubBoxes;
	Rectangle rectangle;
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
	HORIZONTAL;
	VERTICAL;
} Orientation;

BoxList AddInList(BoxList list, Box b);

int LengthBoxList(BoxList list);

Box *BoxListToArray(BoxList list);

void FreeBoxList(BoxList list);

void GetIterPrim(Orientation orient, int *primWidth, int *primHeight);

void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight)

int isBlank(ImageGS img, Box b, guchar c, Orientation orient, int x, int y);

int *GetSpaceArray(ImageGS, Box b, guchar c, Orientation orient, int *size);

void ArraySum(int *arrayA, sizeA, int *arrayB, int sizeB)
{
	if (sizeB > sizeA)
		ArraySum()
}

int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r);

int SpacesVariance(int *spaces, int nbSpaces, int add, double *r);

int ClassifySpace(int *spaces, int nbSpaces, *r);

void Split(ImageGS img, Box *b, Orientation orient, int minBlank);

CutMargin(ImageGS img, Box *b);

Box SplitChars(ImageGS img, Box b, guchar c);//fait

Box SplitWords(ImageGS img, Box b, guchar c);//A FAIRE


Box SplitLines(ImageGS img, Box b, guchar c);//fait

Box SplitBlocks(ImageGS img, Box b, guchar c);//A FAIRE

ImageGS BuildImageGS(int width, int height);

void PrintBoxInformations(Box b);//fait

void PrintImageGS(ImageGS img);//fait mais useless

void Test();//à modifier à loisir pour tester

