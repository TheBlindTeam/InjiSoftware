#include "segmentation.h"
#include "../Utils/image.h"
#include <stdlib.h>
#include <stdio.h>

BoxList AddInList(BoxList list, Box *b)
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

Box **BoxListToArray(BoxList list, int *count)
{
	Box **r = NULL;
	BoxList tmp = list;
	*count = LengthBoxList(list);
	if (*count > 0)
	{
		r = malloc(sizeof(Box*) * *count);
		for (int i = 0; i < *count; i ++)
		{
			r[i] = tmp->value;
			tmp = tmp->next;
		}
		FreeBoxList(list);
	}
	return r;
}

void FreeBoxList(BoxList list)
{
	if (list)
	{
		FreeBoxList(list->next);
		free(list);
	}
}

void FreeBox(Box *b)
{
	for (int i = 0; i < b->nbSubBoxes; i ++)
		FreeBox(b->subBoxes[i]);
	if (b->nbSubBoxes)
		free(b->subBoxes);
	free(b);
}

void GetIterPrim(Orientation orient, int *primWidth, int *primHeight)
{
	*primWidth = orient == VERTICAL ? 1 : 0;
	*primHeight = *primWidth ? 0 : 1;
}

void GetIterSec(Orientation orient, int *secondWidth, int*secondHeight)
{
	GetIterPrim(orient, secondHeight, secondWidth);
}

int isBlank(ImageBN *img, Box *b, Orientation orient, int start)
{
	int x1, x2, y1, y2;
	int min, max;
	int r = 1;

	GetIterPrim(orient, &x1, &y1);
	GetIterSec(orient, &x2, &y2);
	min = b->rectangle.x1 * x2 + b->rectangle.y1 * y2;
	max = b->rectangle.x2 * x2 + b->rectangle.y2 * y2;

	for (int i = min; i <= max && r; i ++)
		r = img->data[start * x1 + i * x2][start * y1 + i * y2] ? 1 : 0;
	return r;
}

void CutMargin(ImageBN *img, Box *b, int V, int H)
{
	int min = b->rectangle.x1;
	int max = b->rectangle.x2;

	if (V)
	{
		for (int i = min; i <= max; i ++)
			if(!isBlank(img, b, VERTICAL, i))
			{
				b->rectangle.x1 = i;
				min = i;
				break;
			}
		for (int i = max; i >= min; i --)
			if(!isBlank(img, b, VERTICAL, i))
			{
				b->rectangle.x2 = i;
				break;
			}
	}
	if (H)
	{
		min = b->rectangle.y1;
		max = b->rectangle.y2;
		for (int i = min; i <= max; i ++)
			if(!isBlank(img, b, HORIZONTAL, i))
			{
				b->rectangle.y1 = i;
				min = i;
				break;
			}
		for (int i = max; i >= min; i --)
			if(!isBlank(img, b, HORIZONTAL, i))
			{
				b->rectangle.y2 = i;
				break;
			}
	}
}

BoxList Split(ImageBN *img, Box *b, Orientation orient, int minBlank)
{
	int x1, y1, x2, y2;
	int count = 0;
	int prev;
	int min;
	int max;
	BoxList list = NULL;

	GetIterPrim(orient, &x1, &y1);
	GetIterSec(orient, &x2, &y2);
	min = x1 * b->rectangle.x1 + y1 * b->rectangle.y1;
	max = x1 * b->rectangle.x2 + y1 * b->rectangle.y2;
	prev = min;
	for (int i = min; i <= max; i ++)
	{
		int isCurBlank = isBlank(img, b, orient, i);
		if (isCurBlank)
			count ++;
		if ((!isCurBlank && count > minBlank) || i == max)
		{
			Box *tmp = malloc(sizeof(Box));
			tmp->rectangle.x1 = x1 * prev + x2 * b->rectangle.x1;
			tmp->rectangle.y1 = y1 * prev + y2 * b->rectangle.y1;
			tmp->rectangle.x2 = x1 *
				(!(!isCurBlank && count >= minBlank) ? i :
					(i ? i - 1 : 0)) + x2 * b->rectangle.x2;
			tmp->rectangle.y2 = y1 *
				(!(!isCurBlank && count >= minBlank) ? i :
					(i ? i - 1 : 0)) + y2 * b->rectangle.y2;
			tmp->nbSubBoxes = 0;
			tmp->subBoxes = NULL;
			prev = i;
			list = AddInList(list, tmp);
		}
		if (!isCurBlank)
			count = 0;
	}
	return list;
}

void GetCharsFromImage(ImageBN *img, Box *b)
{
	BoxList list = Split(img, b, VERTICAL, 1);
	b->subBoxes = BoxListToArray(list, &b->nbSubBoxes);
}

void GetWordsFromImage(ImageBN *img, Box *b)
{
	img = img;
	b = b;
}

void GetLinesFromImage(ImageBN *img, Box *b)
{
	img = img;
	b = b;
}

void GetBlocksFromImage(ImageBN *img, Box *b)
{
	img = img;
	b = b;
}

Box *GetBoxFromSplit(ImageBN *img)
{
	img = img;
	return NULL;
}

Box **GetBreadthBoxArray(Box *b, int *count)
{
	BoxList list = NULL;
	list = AddInList(list, b);
	GetBreadthBoxArrayAux(list);
	return BoxListToArray(list, count);
}

void GetBreadthBoxArrayAux(BoxList list)
{
	for (int i = 0; i < list->value->nbSubBoxes; i ++)
		if (list->value->subBoxes[i]->nbSubBoxes)
			list = AddInList(list, list->value->subBoxes[i]);
	if (list->next)
		GetBreadthBoxArrayAux(list->next);
}

Image *DrawNotInSubBoxes(Image *img, Box *b, Pixel p)
{
	Image *r = ImageCopy(img);
	int **rect = NULL;
	int width = b->rectangle.x2 - b->rectangle.x1 + 1;
	int height = b->rectangle.y2 - b->rectangle.y1 + 1;
	rect = malloc(sizeof(int*) * width);
	for (int i = 0; i < width; i ++)
	{
		rect[i] = malloc(sizeof(int) * height);
		for (int j = 0; j < height; j ++)
			rect[i][j] = 1;
	}
	for (int i = 0; i < b->nbSubBoxes; i ++)
	{
		int subWidth =  b->subBoxes[i]->rectangle.x2
			- b->subBoxes[i]->rectangle.x1 + 1;
		int subHeight =  b->subBoxes[i]->rectangle.y2
			- b->subBoxes[i]->rectangle.y1 + 1;
		int varX = b->subBoxes[i]->rectangle.x1 - b->rectangle.x1;
		int varY = b->subBoxes[i]->rectangle.y1 - b->rectangle.y1;
		for (int j = 0; j < subWidth; j++)
			for (int k = 0; k < subHeight; k++)
				rect[j + varX][k + varY] = 0;	
	}
	for (int i = 0; i < width; i ++)
		for (int j = 0; j < height; j ++)
			if (rect[i][j])
				r->pixList[i + b->rectangle.x1]
					[j + b->rectangle.y1] = p;
	for (int i = 0; i < width; i ++)
		free(rect[i]);
	free(rect);
	return r;
}

Image *DrawBox(Image *img, Box *b, Pixel p)
{
	Image *r = ImageCopy(img);
	for (int i = b->rectangle.x1; i <= b->rectangle.x2; i ++)
	{
		img->pixList[i][b->rectangle.y1] = p;
		img->pixList[i][b->rectangle.y2] = p;
	}
	for (int j = b->rectangle.y1; j <= b->rectangle.y2; j ++)
	{
		img->pixList[b->rectangle.x1][j] = p;
		img->pixList[b->rectangle.x2][j] = p;
	}
	return r;
}
