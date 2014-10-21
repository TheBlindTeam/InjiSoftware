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

int isBlank(ImageGS img, Box b, guchar c, Orientation orient, )

void SetIncrementOrientation(Orientation orient,
	int *primWidth, int *primHeight, int *secondWidth, int *secondHeight)

int *GetSpaceArray(ImageGS, Box b, guchar c, Orientation orient, int *size);

int *ClassifySpace(int *spaces, int nbSpaces, int nbGroup);

double SpacesVariance(int *spaces, int nbSpaces);

double SpacesExpectedValue(int *spaces, int nbSpaces);

Box Split(ImageGs img, Box)

Box SplitChars(ImageGS img, Box b, guchar c);//fait

Box SplitWords(ImageGS img, Box b, guchar c);//A FAIRE


Box SplitLines(ImageGS img, Box b, guchar c);//fait

Box SplitBlocks(ImageGS img, Box b, guchar c);//A FAIRE

ImageGS BuildImageGS(int width, int height);

void PrintBoxInformations(Box b);//fait

void PrintImageGS(ImageGS img);//fait mais useless

void Test();//à modifier à loisir pour tester

