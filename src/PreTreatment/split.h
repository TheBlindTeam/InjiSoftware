#include "../Utils/image.h"
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
	HORIZONTAL;
	VERTICAL;
} Orientation;



BoxList AddInList(BoxList list, Box b);

int LengthBoxList(BoxList list);

Box *BoxListToArray(BoxList list, int *count);

void FreeBoxList(BoxList list);

void GetIterPrim(Orientation orient, int *primWidth, int *primHeight);

void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight);

int isBlank(ImageGS img, Box b, guchar c, Orientation orient, int start);

int *GetSpaceArray(ImageGS, Box b, guchar c, Orientation orient, int *size);

void ArraySum(int *arrayA, sizeA, int *arrayB, int sizeB);

int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r);

int SpacesVariance(int *spaces, int nbSpaces, int add, double *r);

int ClassifySpace(int *spaces, int nbSpaces, int *r);

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

