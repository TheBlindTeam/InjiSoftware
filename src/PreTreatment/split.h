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

typedef struct ElementList
{
	Box value;
	struct ElementList *next;
} ElementList;

typedef ElementList * List;

List Enqueue(List list, Box b, int * count);//fait

Box * ListToArray(List list, int count);//fait

ImageGS BuildImageGS(int width, int height);//fait mais useless

int HorizontalSpaceInfo(ImageGS img, Box b, guchar c);//fait mais mean value

int VerticalSpaceInfo(ImageGS img, Box b, guchar c);//fait mais mean value

Box SplitChars(ImageGS img, Box b, guchar c);//fait

Box SplitWords(ImageGS img, Box b, guchar c);//A FAIRE

void PrintBoxInformations(Box b);//fait

Box SplitLines(ImageGS img, Box b, guchar c);//fait

Box SplitBlocks(ImageGS img, Box b, guchar c);//A FAIRE

void PrintImageGS(ImageGS img);//fait mais useless

void Test();//à modifier à loisir pour tester

