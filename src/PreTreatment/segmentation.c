#include "segmentation.h"
#include "../Utils/image.h"
#include <stdlib.h>
#include <stdio.h>

const Pixel BoxColor[] = {{255, 255, 255, 255}, {255, 255, 0, 255},
	{255, 0, 255, 255}, {0, 255, 255, 255}, {255, 0, 0, 255}, {0, 255, 0, 255},
	{0, 0, 255, 255}};

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

int compareBox(const void *a, const void *b)
{
	Box *A = *(struct Box**)a;
	Box *B = *(struct Box**)b;
	
	if (A->rectangle.y1 != B->rectangle.y1)
		return A->rectangle.y1 - B->rectangle.y1;
	if (A->rectangle.x1 != B->rectangle.x1)
		return A->rectangle.x1 - B->rectangle.x1;
	if (A->rectangle.y2 != B->rectangle.y2)
		return B->rectangle.y2 - A->rectangle.y2;
	return B->rectangle.x2 - A->rectangle.x2;
}

Box *InitBox()
{
	Box *r = malloc(sizeof(Box));
	r->capacity = 1;
	r->nbSubBoxes = 0;
	r->subBoxes = malloc(sizeof(Box*) * r->capacity);
	r->output = NULL;
	r->input = NULL;
	r->charImg = NULL;
	return r;
}

void AddInSubBoxes(Box *b, Box *toBeAdded)
{
	if (b->nbSubBoxes == b->capacity)
	{
		b->capacity *= 2;
		b->subBoxes = realloc(b->subBoxes, sizeof(Box*) * b->capacity);
	}
	b->subBoxes[b->nbSubBoxes] = toBeAdded;
	b->nbSubBoxes++;
}

void FreeBox(Box *b)
{
	for (int i = 0; i < b->nbSubBoxes; i ++)
		FreeBox(b->subBoxes[i]);
	if (b->capacity)
		free(b->subBoxes);
	b->capacity = 0;
	if (b->output)
		free(b->output);
	if (b->input)
		free(b->input);
	if (b->charImg)
		UFreeImageBinary(b->charImg);
	free(b);
}

void GetIterPrim(Orientation orient, int *primWidth, int *primHeight)
{
	*primWidth = orient == VERTICAL ? 1 : 0;
	*primHeight = *primWidth ? 0 : 1;
}

void GetIterSec(Orientation orient, int *secondWidth, int *secondHeight)
{
	GetIterPrim(orient, secondHeight, secondWidth);
}

int isBlank(ImageBN *img, Box *b, Orientation orient, int start, int spaceColor)
{
	int x1, x2, y1, y2;
	int min, max;
	int r = 1;

	GetIterPrim(orient, &x1, &y1);
	GetIterSec(orient, &x2, &y2);
	min = b->rectangle.x1 * x2 + b->rectangle.y1 * y2;
	max = b->rectangle.x2 * x2 + b->rectangle.y2 * y2;

	for (int i = min; i <= max && r; i ++)
		r = img->data[start * x1 + i * x2][start * y1 + i * y2] == spaceColor
			? 1 : 0;
	return r;
}

void CutMargin(ImageBN *img, Box *b, int V, int H, int spaceColor)
{
	int min = b->rectangle.x1;
	int max = b->rectangle.x2;

	if (V)
	{
		for (int i = min; i <= max; i ++)
			if(!isBlank(img, b, VERTICAL, i, spaceColor))
			{
				b->rectangle.x1 = i;
				min = i;
				break;
			}
		for (int i = max; i >= min; i --)
			if(!isBlank(img, b, VERTICAL, i, spaceColor))
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
			if(!isBlank(img, b, HORIZONTAL, i, spaceColor))
			{
				b->rectangle.y1 = i;
				min = i;
				break;
			}
		for (int i = max; i >= min; i --)
			if(!isBlank(img, b, HORIZONTAL, i, spaceColor))
			{
				b->rectangle.y2 = i;
				break;
			}
	}
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
		if (isBlank(img, b, orient, i, 0))
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
	*minVar = expA / expB;
	return 1;
}

int Split(ImageBN *img, Box *b, Box *parent, Orientation orient,
	int minSpace, int spaceColor)
{
	int x1, y1, x2, y2;
	int count = 0;
	int prev;
	int min;
	int max;
	int r = 0;
	int start = 0;

	GetIterPrim(orient, &x1, &y1);
	GetIterSec(orient, &x2, &y2);
	min = x1 * b->rectangle.x1 + y1 * b->rectangle.y1;
	max = x1 * b->rectangle.x2 + y1 * b->rectangle.y2;
	prev = min;
	for (int i = min; i <= max; i ++)
	{
		int isCurBlank = isBlank(img, b, orient, i, spaceColor);
		if (isCurBlank)
			count ++;
		if ((start && !isCurBlank && count > minSpace) || i == max)
		{
			Box *tmp = InitBox();
			tmp->rectangle.x1 = x1 * prev + x2 * b->rectangle.x1;
			tmp->rectangle.y1 = y1 * prev + y2 * b->rectangle.y1;
			tmp->rectangle.x2 = x1 *
				(!(!isCurBlank && count >= minSpace) ? i :
					(i ? i - 1 : 0)) + x2 * b->rectangle.x2;
			tmp->rectangle.y2 = y1 *
				(!(!isCurBlank && count >= minSpace) ? i :
					(i ? i - 1 : 0)) + y2 * b->rectangle.y2;
			prev = i;
			AddInSubBoxes(parent, tmp);
			r++;
		}
		if (!isCurBlank)
		{
			count = 0;
			start = 1;
		}
	}
	return r;
}

int GetCharsFromImage(ImageBN *img, Box *b)
{
	Split(img, b, b, VERTICAL, 1, 0);
	for (int i = 0; i < b->nbSubBoxes; i ++)
	{
		b->subBoxes[i]->lvl = CHARACTER;
		CutMargin(img, b->subBoxes[i], 1, 0, 0);
		b->subBoxes[i]->charImg = ToSquareImage(img, b->subBoxes[i]);
		ImageBN *tmp = ResizeImageBNToChar(b->subBoxes[i]->charImg);
		UFreeImageBinary(b->subBoxes[i]->charImg);
		b->subBoxes[i]->charImg = tmp;
		b->subBoxes[i]->input = ConvertImageToInput(tmp);
		for (int x = 0; x < charInputSize; x ++)
		{
			for (int y = 0; y < charInputSize; y ++)
				if ((int)b->subBoxes[i]->input[x + y * charInputSize])
					printf("X");
				else
					printf(".");
			printf("\n");
		}
	}
	qsort(b->subBoxes, b->nbSubBoxes, sizeof(Box*), compareBox);
	return b->nbSubBoxes;
}

int GetWordsFromImage(ImageBN *img, Box *b)
{
	int *spaces;
	int size;
	double tmp;
	int minBlank;
	int nbChars = 0;
	spaces = GetSpaceArray(img, b, VERTICAL, &size);
	if (!ClassifySpace(spaces, size, &minBlank, &tmp))
		minBlank = 1;
	free(spaces);
	Split(img, b, b, VERTICAL, minBlank, 0);
	for (int i = 0; i < b->nbSubBoxes; i ++)
	{
		b->subBoxes[i]->lvl = WORD;
		CutMargin(img, b->subBoxes[i], 1, 0, 0);
		nbChars += GetCharsFromImage(img, b->subBoxes[i]);
	}
	qsort(b->subBoxes, b->nbSubBoxes, sizeof(Box*), compareBox);
	return nbChars;
}

int GetLinesFromImage(ImageBN *img, Box *b)
{
	int nbChars;
	int nbLines = 0;
	Split(img, b, b, HORIZONTAL, 1, 0);
	for (int i = 0; i < b->nbSubBoxes; i++)
	{
		b->subBoxes[i]->lvl = LINE;
		CutMargin(img, b->subBoxes[i], 1, 1, 0);
		nbChars = GetWordsFromImage(img, b->subBoxes[i]);
		if (nbChars < 1)
		{
			b->subBoxes[i]->lvl = NOTEXT;
			for (int j = 0; j < b->subBoxes[i]->nbSubBoxes; j++)
				FreeBox(b->subBoxes[i]->subBoxes[j]);
			b->nbSubBoxes = 0;
			b->subBoxes[i] = NULL;
			getchar();
		}
		else
			nbLines ++;
	}
	qsort(b->subBoxes, b->nbSubBoxes, sizeof(Box*), compareBox);
	return nbLines;
}

void GetBlocksFromImage(ImageBN *img, ImageBN *mask, Box *b)
{
	int r = 0;
	int max = b->nbSubBoxes;
	int prev = max;
	for (int i = 0; i < max; i++)
	{
		if (Split(mask, b->subBoxes[i], b, HORIZONTAL, 1, 0) > 1)
			r = 1;
		while (prev < b->nbSubBoxes)
			CutMargin(mask, b->subBoxes[prev++], 1, 1, 0);
		FreeBox(b->subBoxes[i]);
		b->subBoxes[i] = b->subBoxes[b->nbSubBoxes - 1];
		b->nbSubBoxes--;
	}
	max = b->nbSubBoxes;
	prev = max;
	if (r)
		for (int i = 0; i < max; i ++)
		{
			if (Split(mask, b->subBoxes[i], b, VERTICAL, 1, 0) > 1)
				r = 2;
			while (prev < b->nbSubBoxes)
				CutMargin(mask, b->subBoxes[prev++], 1, 1, 0);
			FreeBox(b->subBoxes[i]);
			b->subBoxes[i] = b->subBoxes[b->nbSubBoxes - 1];
			b->nbSubBoxes--;
		}
	if (r == 2)
		GetBlocksFromImage(img, mask, b);
	else
	{
		for (int i = 0; i < b->nbSubBoxes; i ++)
		{
			b->subBoxes[i]->lvl = BLOCK;
			if (!GetLinesFromImage(img, b->subBoxes[i]))
			{
				b->subBoxes[i]->lvl = NOTEXT;
				for (int j = 0; j < b->subBoxes[i]->nbSubBoxes; j++)
					FreeBox(b->subBoxes[i]->subBoxes[j]);
				b->subBoxes[i]->nbSubBoxes = 0;
				printf("OHHHHHHHHHH%d %d %d %d\n", b->subBoxes[i]->rectangle.x1
												, b->subBoxes[i]->rectangle.x2
												, b->subBoxes[i]->rectangle.y1
												, b->subBoxes[i]->rectangle.y2);
				b->subBoxes[i] = NULL;
			}
		}
		qsort(b->subBoxes, b->nbSubBoxes, sizeof(Box*), compareBox);
	}
}

Box *GetBoxFromSplit(Image *img)
{
	ImageBN *tmp = URgbToBinary(img);
	ImageBN *bn = NegativeBinaryImage(tmp);
	UFreeImageBinary(tmp);
	ImageBN *dilated = DilatationOnBinary(bn, (img->width + img->height) / 200);
	Box *r = InitBox();
	r->rectangle.x1 = 0;
	r->rectangle.y1 = 0;
	r->rectangle.x2 = bn->width - 1;
	r->rectangle.y2 = bn->height - 1;
	r->lvl = MAIN;
	CutMargin(dilated, r, 1, 1, 0);
	Split(dilated, r, r, HORIZONTAL, 1, 0);
	for (int i = 0; i < r->nbSubBoxes; i ++)
		CutMargin(dilated, r, 1, 1, 0);
	GetBlocksFromImage(bn, dilated, r);
	UFreeImageBinary(bn);
	UFreeImageBinary(dilated);
	return r;
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

Image *DrawBox(Image *img, Box *b, Pixel p, int thickness)
{
	Image *r = ImageCopy(img);
	for (int i = b->rectangle.x1; i <= b->rectangle.x2; i ++)
	{
		for (int j = -thickness / 2; j < thickness / 2; j++)
		{
			if (b->rectangle.y1 + j >= 0 && b->rectangle.y1 + j < img->height)
				r->pixList[i][b->rectangle.y1 + j] = p;
			if (b->rectangle.y2 + j >= 0 && b->rectangle.y2 + j < img->height)
				r->pixList[i][b->rectangle.y2 + j] = p;
		}
	}
	for (int i = b->rectangle.y1; i <= b->rectangle.y2; i ++)
	{
		for (int j = -thickness / 2; j < thickness / 2; j++)
		{
			if (b->rectangle.x1 + j >= 0 && b->rectangle.x1 + j < img->width)
				r->pixList[b->rectangle.x1 + j][i] = p;
			if (b->rectangle.x2 + j >= 0 && b->rectangle.x2 + j < img->width)
				r->pixList[b->rectangle.x2 + j][i] = p;
		}
	}
	return r;
}

Image *DrawWhitePixels(Image *img, ImageBN *mask, Box *b, Pixel p)
{
	Image *r = ImageCopy(img);
	for (int i = b->rectangle.x1; i <= b->rectangle.x2; i++)
		for (int j = b->rectangle.y1; j <= b->rectangle.y2; j++)
			if (mask->data[i][j])
				r->pixList[i][j] = p;
	return r;
}

Image *DrawBlackPixels(Image *img, ImageBN *mask, Box *b, Pixel p)
{
	printf("%d %p\n", b->lvl, b->input);
	ImageBN *tmp = NegativeBinaryImage(mask);
	Image *r = DrawWhitePixels(img, tmp, b, p);
	UFreeImageBinary(tmp);
	return r;
}
