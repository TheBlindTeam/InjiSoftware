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

void GetIterPrim(Orientation orient, int *primWidth, int *primHeight);

void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight);

int isBlank(ImageGS img, Box b, guchar c, Orientation orient, int start);

int *GetSpaceArray(ImageGS, Box b, guchar c, Orientation orient, int *size);

int *ArraySum(int *arrayA, int sizeA, int *arrayB, int sizeB, int *newSize);

int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r);

int SpacesVariance(int *spaces, int nbSpaces, int add, double *r);

int ClassifySpace(int *spaces, int nbSpaces, int *r, double *min);

void Split(ImageGS img, Box *b, Orientation orient, int minBlank, guchar c);

void CutMargin(ImageGS img, Box *b, guchar c);

void SplitChars(ImageGS img, Box *b, guchar c);

void SplitWords(ImageGS img, Box *b, guchar c, int minBlank);

void SplitLines(ImageGS img, Box *b, guchar c);

void SplitBlocks(ImageGS img, Box *b, guchar c, int minBlank);

Box *GetBreadthBoxArray(Box b, int *count);

void GetBreadthBoxArrayAux(BoxList list);

void DrawNotInSubBoxes(Image img, Box b);
