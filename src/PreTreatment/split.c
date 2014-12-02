#include "split.h"
#include "../Utils/image.h"
#include <stdlib.h>
#include <stdio.h>

const guchar BLACKGS = 200;

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

int *GetSpaceArray(ImageBN *img, Box *b, Orientation orient, int *size)
{
	int x1, y1;
	int *r;
	int count = 0;
	int max;
	int min;
	GetIterPrim(orient, &x1, &y1);
	min = b->rectangle.x1 * x1 + b->rectangle.y1 * y1;
	max = b->rectangle.x2 * x1 + b->rectangle.y2 * y1;
	*size = max - min + 1;
	r = malloc(sizeof(int) * *size);
	for (int i = 0; i < *size; i ++)
		r[i] = 0;
	for (int i = min; i <= max; i ++)
	{
		if (isBlank(img, b, orient, i))
			count ++;
		else if (count > 1)
		{
			r[count - 1]++;
			count = 0;
		}
		if (count == *size)
			r[count - 1]++;
	}
	return r;
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
		sum += spaces[i] * pow(1 - ((i + 1 + add) < expectedValue ? 
			(double)(i + 1 + add) / (double)expectedValue :
			(double)expectedValue / (double)(i + 1 + add)), 2);
		count += spaces[i];
	}
	if (count)
	{
		*r = sum / (double)count;
		return 1;
	}
	return 0;
}

int ClassifySpace(int *spaces, int nbSpaces, int *r, double *minVar)
{
	double tmpA = 0;
	double tmpB = 0;
	double expA = 0;
	double expB = 0;
	double min;
 	if(!SpacesVariance(spaces, nbSpaces, 0, &tmpB))
		return 0;
	min = pow(tmpB, 2);
	*r = 0;
	for (int i = 1; i < nbSpaces - 1; i ++)
		if (spaces[i] > 0)
			if (SpacesVariance(spaces, i + 1, i, &tmpA) &&
				SpacesVariance(&spaces[i + 1],
					nbSpaces - i - 1, i + 1, &tmpB))
				if (pow(tmpA, 2) + pow(tmpB, 2) < min)
				{
					min = pow(tmpB, 4) + pow(tmpA, 4);
					*r = i + 1;
					SpacesExpectedValue(spaces, i + 1, i, &expA);
					SpacesExpectedValue(spaces, nbSpaces - i - 1,
						i + 1, &expB);
				}
	*minVar = pow(1 - (expA / expB), 2);
	return 1;
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

void Split(ImageBN *img, Box *b, Orientation orient, int minBlank, int state)
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
			if (state == 0 || state == 1)
				CutMargin(img, tmp, 1, 1);
			else
				CutMargin(img, tmp, 1, 0);
			list = AddInList(list, tmp);
		}
		if (!isCurBlank)
			count = 0;
	}
	b->subBoxes = BoxListToArray(list, &(b->nbSubBoxes));
	for (int i = 0; i < b->nbSubBoxes; i ++)
	{
		if (state == 0)
			DetectSplitOrientation(img, b->subBoxes[i]);
		else if (state == 1 || state == 2)
		{
			int size = 0;
			double var = 0;
			int *spaces = GetSpaceArray(img, b->subBoxes[i],
				VERTICAL, &size);
			int set = ClassifySpace(spaces, size, &minBlank, &var);
			free(spaces);
			if (set && minBlank > 0 && var > 0.02)
				Split(img, b->subBoxes[i], VERTICAL, minBlank, 2);
			else
				Split(img, b->subBoxes[i], VERTICAL, 0, 3);
		}
	}
}

void DetectSplitOrientation(ImageBN *img, Box *b)
{
	int sizeH;
	int *spacesH = GetSpaceArray(img, b, HORIZONTAL, &sizeH);
	int sizeV;
	int *spacesV = GetSpaceArray(img, b, VERTICAL, &sizeV);
//	int *spaces;
//	int set = 0;
//	int setB = 0;
	int minBlankH;
	int minBlankV;
	double varH;
	double varV;
//	Orientation o;
	int H = ClassifySpace(spacesH, sizeH, &minBlankH, &varH);
	int V = ClassifySpace(spacesV, sizeV, &minBlankV, &varV);
	/*
	for (int i = sizeH > sizeV ? sizeH : sizeV; i >= 0 && !set; i --)
	{
		int H = i < sizeH ? spacesH[i] : 0;
		int V = i < sizeV ? spacesV[i] : 0;
		if (H > V)
		{
			o = HORIZONTAL;
			spaces = spacesH;
			size = sizeH;
			free(spacesV);
			set = 1;
		}
		else if (H < V)
		{
			o = VERTICAL;
			spaces = spacesV;
			size = sizeV;
			free(spacesH);
			set = 1;
		}
		else if (H)
			setB = 1;
	}*/
	if (V && minBlankV > 0 && varV > 0.08 && (!H || varV > varH))
		Split(img, b, VERTICAL, minBlankV, 0);
	else if (H && varH > 0.08 && minBlankH > 0)
		Split(img, b, HORIZONTAL, minBlankH, 0);
	else
		Split(img, b, HORIZONTAL, 0, 1);
	/*if (set)
	{
		set = ClassifySpace(spaces, size, &minBlank, &var);
		free(spaces);
		if (set && minBlank > 1 && var > 0.3)
			Split(img, b, o, minBlank, 0);
		else
			Split(img, b, HORIZONTAL, 1, 1);
	}
	else if (setB)
		Split(img, b, HORIZONTAL, 1, 1);
*/
}

Box *GetBoxFromSplit(Image *img)
{
	ImageBN *imgBn = URgbToBinary(img);
	Box *b = malloc(sizeof(Box));
	b->rectangle.x1 = 0;
	b->rectangle.y1 = 0;
	b->rectangle.x2 = imgBn->width - 1;
	b->rectangle.y2 = imgBn->height - 1;
	Box *sub = malloc(sizeof(Box));
	b->nbSubBoxes = 1;
	b->subBoxes = malloc(sizeof(Box*));
	*b->subBoxes = sub;
	sub->rectangle = b->rectangle;
	sub->nbSubBoxes = 0;
	sub->subBoxes = NULL;
	CutMargin(imgBn, sub, 1, 1);
	DetectSplitOrientation(imgBn, sub);
	UFreeImageBinary(imgBn);
	return b;
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

void DrawNotInSubBoxes(Image *img, Box *b, Pixel p)
{
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
				img->pixList[i + b->rectangle.x1]
					[j + b->rectangle.y1] = p;
	for (int i = 0; i < width; i ++)
		free(rect[i]);
	free(rect);
}
