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

void SetIncrementOrientation(Orientation orient,
	int *primWidth, int *primHeight, int *secondWidth, int *secondHeight)
{
	int primWidth = orient == ORIENTATION.VERTICAL ? 1 : 0;
	int primHeight = primWidth ? 0 : 1;
	int secondWidth = primHeight;
	int secondHeight = primWidth;
}

int *GetSpaceArray(ImageGS img, Box b, guchar c, Orientation orient, int *size)
{
	int x1, y1, x2, y2;
	int *r;
	int count;
	SetIncrementOrientation(orient, &x1, &y1, &x2, &y2);
	*size = b.width * x1 + b.height * y1;
	r = malloc(sizeof(int) * b->width * x1 + b->height * y1);
	
}

List Enqueue(List list, Box b, int * count)
{
	ElementList * elt = malloc(sizeof(ElementList));
	elt->value = b;
	elt->next = list;
	(*count)++;
	printf("valeur de count : %d\n", *count);
	return elt;
}

Box * ListToArray(List list, int count)
{
	ElementList * temp = list;
	Box * b = malloc(sizeof(Box)*count);
	int i = count - 1;
	while(i >= 0 && temp != NULL)
	{
		b[i] = temp->value;
		temp = temp->next;
		i--;
	}
	return b;
}



//#flemmeDeDeplacerLeCode
ImageGS BuildImageGS(int width, int height);
int VerticalSpaceInfo(ImageGS img, Box b, guchar c);



int HorizontalSpaceInfo(ImageGS img, Box b, guchar c)
/*
Renvoie la moyenne des espaces entre les lignes, sans prendre en compte les espaces
aux extrémités (qui peuvent être très grands).
*/
{
	int nbSpaces = 0, total = 0, firstSpace;
	int inLine = 0;
	int space = 0;
	for (int i = b.rectangle.y; i < b.rectangle.y + b.rectangle.height; i++)
	{
		int j = b.rectangle.x;
		while (j < b.rectangle.x + b.rectangle.width && img.intensity[i][j] >= c)
			j++;
		if (!inLine && j != b.rectangle.width)
			inLine = 1;
		if (inLine && j == b.rectangle.width)
		{
			inLine = 0;
			firstSpace = i;
		}
	}

	for (int i = firstSpace; i < b.rectangle.y + b.rectangle.height; i++)
	{
		int j = b.rectangle.x;
		while (j < b.rectangle.x + b.rectangle.width && img.intensity[i][j] >= c)
			j++;
		if (!inLine && j != b.rectangle.width)
			inLine = 1;
			total += space;
			space = 0;
			nbSpaces++;
		if (inLine && j == b.rectangle.width)
		{
			inLine = 0;
			space++;
		}
	}
	return total / nbSpaces;
}



int VerticalSpaceInfo(ImageGS img, Box b, guchar c)
{
	int nbSpaces = 0, total = 0, firstSpace;
	int inLine = 0;
	int space = 0;
	for (int i = b.rectangle.x; i < b.rectangle.x + b.rectangle.width; i++)
	{
		int j = b.rectangle.y;
		while (j < b.rectangle.y + b.rectangle.height && img.intensity[j][i] >= c)
			j++;
		if (!inLine && j != b.rectangle.height)
			inLine = 1;
		if (inLine && j == b.rectangle.height)
		{
			inLine = 0;
			firstSpace = i;
		}
	}

	for (int i = firstSpace; i < b.rectangle.x + b.rectangle.width; i++)
	{
		int j = b.rectangle.y;
		while (j < b.rectangle.y + b.rectangle.height && img.intensity[j][i] >= c)
			j++;
		if (!inLine && j != b.rectangle.height)
			inLine = 1;
			total += space;
			space = 0;
			nbSpaces++;
		if (inLine && j == b.rectangle.width)
		{
			inLine = 0;
			space++;
		}
	}
	return total / nbSpaces;
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
