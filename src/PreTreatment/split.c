#include "split.h"
#include "../Utils/image.h"
#include <stdlib.h>
#include <stdio.h>

/*Nota bene : 
J'ai considéré l'image comme ayant un repère d'axe x VERTICAL orienté vers 
le bas et d'axe y HORIZONTAL orienté vers la gauche*/
const guchar BLACKGS = 10;

BoxList AddInList(BoxList list, Box b)
{
	BoxElementList *elt = malloc(sizeof(BoxElementList));
	elt->value = b;
	elt->next = NULL;
	BoxList tmp = list;
	if (!list)
		return elt;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = elt;
	return list;
}

int LengthBoxList(BoxList list)
{
	int count = 0;
	while (list)
	{
		list = list->next;
		count ++;
	}
	return count;
}

Box *BoxListToArray(BoxList list, int *count)
{
	Box *r;
	BoxList tmp = list;
	r = malloc(sizeof(Box) * *count);
	*count = LengthBoxList(list);
	for (int i = 0; i < *count; i ++)
	{
		r[i] = tmp->value;
		tmp = tmp->next;
	}
	FreeBoxList(list);
	return r;
}

void FreeBoxList(BoxList list)
{
	if (list->next)
		FreeBoxList(list->next);
	free(list);
}

void GetIterPrim(Orientation orient, int *primWidth, int *primHeight)
{
	*primWidth = orient == VERTICAL ? 1 : 0;
	*primHeight = primWidth ? 0 : 1;
}

void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight)
{
	GetIterPrim(orient, secondHeight, secondWidth);
}

int isBlank(ImageGS img, Box b, guchar c, Orientation orient, int start)
{
	int x1, x2, y1, y2;
	int min, max;
	int r = 1;

	GetIterPrim(orient, &x1, &y1);
	GetIterSec(orient, &x2, &y2);
	min = b.rectangle.x1 * x2 + b.rectangle.y1 * y2;
	max = b.rectangle.x2 * x2 + b.rectangle.y2 * y2;
	for (int i = min; i <= max && r; i ++)
		r = img.intensity[start * x1 + i * x2][start * y1 + i * y2] > c;
	return r;
}

int *GetSpaceArray(ImageGS img, Box b, guchar c, Orientation orient, int *size)
{
	int x1, y1;
	int *r;
	int count = 0;
	int max;
	int min;
	GetIterPrim(orient, &x1, &y1);
	min = b.rectangle.x1 * x1 + b.rectangle.y1 * y1;
	max = b.rectangle.x2 * x1 + b.rectangle.y2 * y1;
	*size = max - min + 1;
	r = malloc(sizeof(int) * *size);
	for (int i = min; i <= max; i ++)
	{
		r[i] = 0;
		if (isBlank(img, b, c, orient, i))
			count ++;
		else
		{
			r[count - 1]++;
			count = 0;
		}
	}
	printf("Space Array Info size: %d\n", *size);
	for (int i = 0; i < *size; i ++)
	{
		printf("space %d count %d\n", i +1, r[i]);
	}
	return r;
}

int *ArraySum(int *arrayA, int sizeA, int *arrayB, int sizeB, int *newSize)
{
	if (sizeB > sizeA)
		return ArraySum(arrayB, sizeB, arrayA, sizeA, newSize);
	if (arrayB)
	{
		for (int i = 0; i < sizeB; i++)
			arrayA[i] += arrayB[i];
		free(arrayB);
	}
	*newSize = sizeA;
	return arrayA;
}

int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r)
{
	int count = 0;
	int sum = 0;
	for (int i = 0; i < nbSpaces; i ++)
	{
		sum += spaces[i] * (i + 1 + add);
		count += spaces[i];
	}
	if (count)
	{
		*r = (double)sum / (double)count;
		return 1;
	}
	return 0;
}

int SpacesVariance(int *spaces, int nbSpaces, int add, double *r)
{
	double expectedValue = 0;
	double sum = 0;
	int count = 0;
	if(!SpacesExpectedValue(spaces, nbSpaces, add, &expectedValue))
		return 0;
	for (int i = 0; i < nbSpaces; i ++)
	{
		sum += spaces[i] * pow(expectedValue - (i + 1 + add), 2);
		count += spaces[i];
	}
	if (count)
	{
		*r = sum / (double)count;
		return 1;
	}
	return 0;
}

int ClassifySpace(int *spaces, int nbSpaces, int *r, double *min)
{
	double tmpA = 0;
	double tmpB = 0;
	printf("Classify 1\n");
 	if(!SpacesVariance(spaces, nbSpaces, 0, &tmpB))
		return 0;
	printf("Classify 2\n");
	*min = pow(tmpB, 2);
	*r = 0;
	printf("Classify 3\n");
	for (int i = 1; i < nbSpaces - 1; i ++)
		if (spaces[i] > 0)
			if (SpacesVariance(spaces, i, i, &tmpA) &&
				SpacesVariance(&spaces[i + 1], nbSpaces - i, i + 1, &tmpB) &&
				pow(tmpB, 2) + pow(tmpA, 2) < *min)
			{
				*min = pow(tmpB,2) + pow(tmpA, 2);
				*r = i + 1;
			}
	printf("Classify 4\n");
	return 1;
}

void CutMargin(ImageGS img, Box *b, guchar c)
{
	int min = b->rectangle.x1;
	int max = b->rectangle.x2;

	for (int i = min; i <= max; i ++)
		if(!isBlank(img, *b, c, VERTICAL, i))
		{
			b->rectangle.x1 = i;
			min = i;
			break;
		}
	for (int i = max; i >= min; i --)
		if(!isBlank(img, *b, c, VERTICAL, i))
		{
			b->rectangle.x2 = i;
			break;
		}
	min = b->rectangle.y1;
	max = b->rectangle.y2;
	for (int i = min; i <= max; i ++)
		if(!isBlank(img, *b, c, HORIZONTAL, i))
		{
			b->rectangle.y1 = i;
			min = i;
			break;
		}
	for (int i = max; i >= min; i --)
		if(!isBlank(img, *b, c, HORIZONTAL, i))
		{
			b->rectangle.y2 = i;
			break;
		}
}

void Split(ImageGS img, Box *b, Orientation orient, int minBlank, guchar c)
{

	int x1, y1, x2, y2;
	int count = 0;
	int prev = 0;
	int min;
	int max;
	BoxList list = NULL;

	GetIterPrim(orient, &x1, &y1);
	GetIterPrim(orient, &x2, &y2);
	min = x1 * b->rectangle.x1 + y1 * b->rectangle.y1;
	max = x1 * b->rectangle.x2 + y1 * b->rectangle.y2;
	for (int i = min; i <= max; i ++)
		if (isBlank(img, *b, c, orient, i))
			count ++;
		else
		{
			count = 0;
			if (count >= minBlank)
			{
				Box tmp;
				tmp.rectangle.x1 = x1 * prev + x2 * b->rectangle.x1;
				tmp.rectangle.y1 = y1 * prev + y2 * b->rectangle.y1;
				tmp.rectangle.x2 = x1 * i + x2 * b->rectangle.x2;
				tmp.rectangle.y2 = y1 * i + y2 * b->rectangle.y2;
				tmp.nbSubBoxes = 0;
				tmp.subBoxes = NULL;
				prev = i;
				CutMargin(img, &tmp, c);
				list = AddInList(list, tmp);
			}
		}
	b->subBoxes = BoxListToArray(list, &b->nbSubBoxes);
}

void SplitChars(ImageGS img, Box *b, guchar c)
{
	Split(img, b, VERTICAL, 1, c);
}

void SplitWords(ImageGS img, Box *b, guchar c, int minBlank)
{
	Split(img, b,VERTICAL, minBlank, c);
	for (int i = 0; i < b->nbSubBoxes; i ++)
		SplitChars(img, &b->subBoxes[i], c);
}

void SplitLines(ImageGS img, Box *b, guchar c)
{
	Split(img, b, HORIZONTAL, 1, c);
	int *arrayA = NULL;
	int *arrayB = NULL;
	int sizeA = 0;
	int sizeB = 0;
	int minBlank = 0;
	double minVar = 0;

	for (int i = 0; i < b->nbSubBoxes; i ++)
	{
		arrayB = GetSpaceArray(img, b->subBoxes[i], c, VERTICAL, &sizeB);
		arrayA = ArraySum(arrayA, sizeA, arrayB, sizeB, &sizeA);
	}
	if (ClassifySpace(arrayA, sizeA, &minBlank, &minVar)
		&& minVar < 1.0 && minBlank > 1)
	{
		for (int i = 0; i < b->nbSubBoxes; i ++)
			SplitWords(img, &b->subBoxes[i], c, minBlank);
	}
	else
		for (int i = 0; i < b->nbSubBoxes; i ++)
			SplitChars(img, &b->subBoxes[i], c);
}

void SplitBlocks(ImageGS img, Box *b, guchar c, int minBlank)
{
	printf("Split Blocks");
	Split(img, b, HORIZONTAL, minBlank, c);
	for (int i = 0; i < b->nbSubBoxes; i ++)
		SplitLines(img, &b->subBoxes[i], c);
}

Box GetBoxFromSplit(Image img)
{
	int minBlank = 0;
	int tmp = 0;
	double minVar = 0;
	printf("1\n");
	ImageGS imgGs = URgbToGrayscale(img);
	Box b = {{0, 0, img.width - 1, img.height - 1}, 0, NULL};
	Box *sub = malloc(sizeof(Box));
	b.nbSubBoxes = 1;
	b.subBoxes = sub;
	sub->rectangle = b.rectangle;
	sub->nbSubBoxes = 0;
	sub->subBoxes = NULL;
	printf("2\n");
	CutMargin(imgGs, sub, BLACKGS);
	printf("3\n");
	if (ClassifySpace(
		GetSpaceArray(imgGs, *sub, BLACKGS, HORIZONTAL, &tmp),
		tmp, &minBlank, &minVar)
		&& minVar < 1.0 && minBlank > 1)
		SplitBlocks(imgGs, sub, BLACKGS, minBlank);
	else
		SplitLines(imgGs, sub, BLACKGS);
	printf("4\n");
	return b;
}

Box *GetBreadthBoxArray(Box b, int *count)
{
	BoxList list = NULL;
	list = AddInList(list, b);
	GetBreadthBoxArrayAux(list);
	return BoxListToArray(list, count);
}

void GetBreadthBoxArrayAux(BoxList list)
{
	for (int i = 0; i < list->value.nbSubBoxes; i ++)
		if (list->value.subBoxes[i].nbSubBoxes)
			list = AddInList(list, list->value.subBoxes[i]);
	if (list->next)
		GetBreadthBoxArrayAux(list->next);
}

void DrawNotInSubBoxes(Image img, Box b, Pixel p)
{
	int **rect;
	int width = b.rectangle.x2 - b.rectangle.x1 + 1;
	int height = b.rectangle.y2 - b.rectangle.y1 + 1;
	rect = malloc(sizeof(int) * width);
	for (int i = 0; i < width; i ++)
	{
		rect = malloc(sizeof(int) * height);
		for (int j = 0; j < height; j ++)
			rect[i][j] = 1;
	}
	for (int i = 0; i < b.nbSubBoxes; i ++)
	{
		int subWidth =  b.subBoxes[i].rectangle.x2
			- b.subBoxes[i].rectangle.x1 + 1;
		int subHeight =  b.subBoxes[i].rectangle.y2
			- b.subBoxes[i].rectangle.y1 + 1;
		int varX = b.subBoxes[i].rectangle.x1 - b.rectangle.x1;
		int varY = b.subBoxes[i].rectangle.y1 - b.rectangle.y1;
		for (int j = 0; j < subWidth; j++)
			for (int k = 0; k < subHeight; k++)
				rect[j + varX][k + varY] = 0;
	}
	for (int i = 0; i < width; i ++)
		for (int j = 0; j < height; j ++)
			if (rect[i][j])
				img.pixList[i + b.rectangle.x1][j + b.rectangle.y1] = p;
}
