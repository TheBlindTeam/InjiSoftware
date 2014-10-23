#include "split.h"
#include "../Utils/image.h"
#include <stdlib.h>
#include <stdio.h>

/*Nota bene : 
J'ai considéré l'image comme ayant un repère d'axe x VERTICAL orienté vers 
le bas et d'axe y HORIZONTAL orienté vers la gauche*/

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
	BoxList tmp = list
	r = malloc(sizeof(count));
	*count = LengthBoxList(list);
	for (int i = 0; i < *count; i ++)
	{
		r[i] = tmp->elt;
		tmp = tmp->next;
	}
	FreeBoxList(list);
}

void FreeBoxList(BoxList list)
{
	if (list->next)
		FreeBoxList(list->next);
	free(list);
}

void GetIterPrim(Orientation orient, int *primWidth, int *primHeight)
{
	int *primWidth = orient == ORIENTATION.VERTICAL ? 1 : 0;
	int *primHeight = primWidth ? 0 : 1;
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

	GetIterPrim(orient, &x1, &x2);
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
	GetIterPrim(orient, &x1, &x2);
	min = b.rectangle.x1 * x2 + b.rectangle.y1 * y2;
	max = b.rectangle.x2 * x2 + b.rectangle.y2 * y2;
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
	return r;
}

void ArraySum(int *arrayA, sizeA, int *arrayB, int sizeB)
{
	if (sizeB > sizeA)
		ArraySum(arrayB, sizeB, arrayA, sizeA);
	for (int i = 0; i < sizeB; i++)
		arrayA[i] += arrayB[i];
	free(arrayB);
}

int SpacesExpectedValue(int *spaces, int nbSpaces, int add, double *r)
{
	int count = 0;
	int sum = 0;
	for (int i = 0; i < nbSpaces; i ++)
	{
		sum += spaces[i] * (i + 1 + add)
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
		sum += spaces[i] * pow((expectedValue - (i + 1 + add), 2));
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
 	if(!SpacesVariance(spaces, nbSpaces, 0, tmpB));
		return 0;
	*min = pow(tmpB, 2);
	*r = 0;
	for (int i = 1; i < nbSpaces - 1; i ++)
		if (spaces[i] > 0)
			if (SpacesVariance(spaces, i, i, &tmpA) &&
				SpacesVariance(&spaces[i + 1], nbSpaces - i, i + 1, &tmpB) &&
				pow(tmpB, 2) + pow(tmpA, 2) < *min)
			{
				*min = pow(tmpB,2) + pow(tmpA, 2);
				*r = i + 1;
			}
	return 1;
}

CutMargin(ImageGS img, Box *b, guchar c)
{
	int var;

	int min = b->rectangle.x1;
	int max = b->rectangle.x2;
	for (int i = min; i <= max; i ++)
		if(!isBlank(img, b, c, ORIENTATION.VERTICAL, i))
		{
			b->rectangle.x1 = i;
			break;
		}
	for (int i = b->rectangle.width; i >= 0; i --)
		if(!isBlank(img, b, c, ORIENTATION.VERTICAL, i + var))
		{
			b->rectangle.width = i;
			break;
		}
	var = b->rectangle.y;
	for (int i = 0; i < b->rectangle.height; i ++)
		if(!isBlank(img, b, c, ORIENTATION.HORIZONTAL, i + var))
		{
			b->rectangle.y += i;
			var += i;
			b->rectangle.height -= i;
			break;
		}
	for (int i = b->rectangle.height; i >= 0; i --)
		if(!isBlank(img, b, c, ORIENTATION.VERTICAL, i + var))
		{
			b->rectangle.height = i;
			break;
		}
}

void Split(ImageGS img, Box *b, Orientation orient, minBlank)
{

	int x1, y1;
	int *r;
	int count = 0;
	int prev;
	BoxList list = NULL;

	GetIterPrim(orient, &x1, &y1);
	*size = b.rectangle.width * x1 + b.rectangle.height * y1;
	r = malloc(sizeof(int) * *size);
	var = b.rectangle.x * x1 + b.rectangle.y * y1;
	prev = 0;
	for (int i = 0; i < *size; i ++)
	{
		r[i] = 0;
		if (isBlank(img, b, c, orient, i + var))
			count ++;
		else
		{
			count = 0;
			if (count >= minBlank)
			{
				box tmp;
				tmp.rectangle.x = x1 * prev + b->rectangle.x;
				tmp.rectangle.y = y1 * prev + b->rectangle.y;
				tmp.rectangle.width = x1 * (i - prev) + x2 * b->rectangle.width;
				tmp.rectangle.height = y1 * (i - prev) + y2 * b->rectangle.height;
				
			list = AddInList(list, )
		}
	}
	return r;
}

Box GetBoxFromSplit(Image img)
{
	Box r;
	r.rectangle.x = 0;
	r.rectangle.y;
	r.rectangle.width = img
}

Box *GetBreadthBoxArray(Box b, int *count)
{
	BoxList list;
	list = AddInList(list, b);
	GetBreadthBoxArrayAux(&list);
	return BoxListToArray(list, count);
}

void GetBreadthBoxArrayAux(BoxList *list)
{
	for (int i = 0; i < list->value.nbSubBoxes; i ++)
		if (list->value.subBoxes[i].nbSubBoxes)
			list = AddInList(list, list->value.subBoxes[i]);
	if (list->next)
		GetBreadthBoxArray(list->next);
}

void DrawNotInSubBoxes(Image img, Box b)
{
	int **rect;
	rect = malloc(sizeof(int) * b.width);
	for (int i = 0; i < b.rectangle.width; i ++)
	{
		rect = malloc(sizeof(int) * b.height);
		for (int j = 0; j < b.rectangle.height; j ++);
			rect[i][j] = 1;
	}
	for (int i = 0; i < size; i ++)
	{
		int varX = b.subBoxes[i].rectangle x - b.rectangle.x;
		int varY = b.subBoxes[i].rectangle.y - b.rectangle.y;
		for (int j = 0; k < varX + b.subBoxes[i].width; j++)
			for (int k = 0; k < varY + b.subBoxes[i].height; k++)
				rect[j][k] = 0;
	}
	for (int i = 0; i < b.rectangle.width; i ++)
		for (int j = 0; j < b.rectangle.height; j ++)
			if (rect[i][j])
				img.pixList[i + b.rectangle.x][j + b.rectangle.y] = RED;
}

Box SplitChars(ImageGS img, Box b, guchar c)
{
	int count = 0;
	List listBoxes = NULL;
	printf("Initialization list\n");
	int inLine = 0; 
	for (int j = b.rectangle.x; j < b.rectangle.x + b.rectangle.width; j++)
	{
		int i = b.rectangle.y;
		//NOTE : structure d'image en grey scale à faire
		while (i < b.rectangle.y + b.rectangle.height && img.intensity[i][j] >= c)
			i++;
		if (!inLine && i != b.rectangle.y + b.rectangle.height)
		{
			inLine = 1;
			Box btemp;
			btemp.rectangle.x = j;
			btemp.rectangle.y = b.rectangle.y;
			btemp.rectangle.height = b.rectangle.height;
			listBoxes = Enqueue(listBoxes, btemp, &count);
			//printf("valeur x : %d\n", listBoxes->value.rectangle.x);
		}
		if (inLine && i == b.rectangle.y + b.rectangle.height)
		{
			inLine = 0;
			listBoxes->value.rectangle.width = j - listBoxes->value.rectangle.x;
			PrintBoxInformations(listBoxes->value);
		}
	}
	b.subBoxes = ListToArray(listBoxes, count);
	printf("nb chars : %d\n", count);
	return b;
}

Box SplitWords(ImageGS img, Box b, guchar c);



void PrintBoxInformations(Box b)
{
	printf("x = %d ; y = %d ; w = %d ; h = %d.\n", b.rectangle.x, b.rectangle.y, b.rectangle.width, b.rectangle.height);
}



Box SplitLines(ImageGS img, Box b, guchar c)
{
	int count = 0;
	List listBoxes = NULL;
	printf("Initialization list\n");
	int inLine = 0; 
	for (int i = b.rectangle.y; i < b.rectangle.y + b.rectangle.height; i++)
	{
		int j = b.rectangle.x;
		//NOTE : structure d'image en grey scale à faire
		while (j < b.rectangle.x + b.rectangle.width && img.intensity[i][j] >= c)
			j++;
		if (!inLine && j != b.rectangle.x + b.rectangle.width)
		{
			inLine = 1;
			Box btemp;
			btemp.rectangle.x = b.rectangle.x;
			btemp.rectangle.y = i;
			btemp.rectangle.width = b.rectangle.width;
			listBoxes = Enqueue(listBoxes, btemp, &count);
			//printf("valeur x : %d\n", listBoxes->value.rectangle.x);
		}
		if (inLine && j == b.rectangle.x + b.rectangle.width)
		{
			inLine = 0;
			listBoxes->value.rectangle.height = i - listBoxes->value.rectangle.y;
		PrintBoxInformations(listBoxes->value);
		}
	}
	b.subBoxes = ListToArray(listBoxes, count);
	printf("nb lines : %d\n", count);
	return b;
}

Box SplitBlocks(ImageGS img, Box b, guchar c);


ImageGS BuildImageGS(int width, int height)
{
//Initialisation
	ImageGS img;
	img.width = width;
	img.height = height;
	img.intensity = malloc(sizeof(guchar*) * height);
	for (int i = 0; i < height; i++)
		img.intensity[i] = malloc(sizeof(guchar) * width);

//Remplissage
	for (int j = 0; j < img.width; j++)
		for (int i = 0; i <= 2; i++)
			img.intensity[i][j] = 255;

	for (int i = 3; i < img.height/2 - 1; i++)
	{
		for (int j = 0; j <= 2; j++)
			img.intensity[i][j] = 255;
		for (int j = 3; j < img.width/2 - 1; j++)
			img.intensity[i][j] = 0;
		for (int j = img.width/2 - 1; j <= img.width/2 + 1; j++)
			img.intensity[i][j] = 255;
		for (int j = img.width/2 + 2; j < img.width - 3; j++)
			img.intensity[i][j] = 0;
		for (int j = img.width - 3; j < img.width; j++)
			img.intensity[i][j] = 255;
	}

	for (int j = 0; j < img.width; j++)
		for (int i = img.height/2 - 1; i <= img.height/2 + 1; i++)
			img.intensity[i][j] = 255;

	for (int i = img.height/2 + 2; i < img.height - 3; i++)
	{
		for (int j = 0; j <= 2; j++)
			img.intensity[i][j] = 255;
		for (int j = 3; j < img.width/2 - 1; j++)
			img.intensity[i][j] = 0;
		for (int j = img.width/2 - 1; j <= img.width/2 + 1; j++)
			img.intensity[i][j] = 255;
		for (int j = img.width/2 + 2; j < img.width - 3; j++)
			img.intensity[i][j] = 0;
		for (int j = img.width - 3; j < img.width; j++)
			img.intensity[i][j] = 255;
	}

	for (int j = 0; j < img.width; j++)
		for (int i = img.height - 3; i < img.height; i++)
			img.intensity[i][j] = 255;

	return img;
}

void PrintImageGS(ImageGS img)
{
	for (int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			if (img.intensity[i][j] >= 128)
				printf("-");
			else
				printf("8");
		}
		printf("\n");
	}
}

void Test()
{
	ImageGS img = BuildImageGS(41,21);
	PrintImageGS(img);
	Box box;
	box.rectangle.x = 0;
	box.rectangle.y = 0;
	box.rectangle.width = 41;
	box.rectangle.height = 21;
	SplitLines(img, box, 128);
}
