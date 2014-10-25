#include "matrixOperation.h"

Pixel** UMultiply(Pixel **matrix, double **convolution, int matrixSize)
{
	Pixel **tmp;

	tmp = malloc(matrixSize * sizeof(Pixel *));

	for (int i = 0; i < matrixSize; i++)
	{
		tmp[i] = malloc(matrixSize * sizeof(Pixel));
	}	

	for (int y = 0; y < matrixSize; y++)
	{
		for (int x = 0; x < matrixSize; x++)
		{
			for(int i = 0; i < x; i++)
			{
				tmp[x][y].r += (matrix[i][y].r *
					(guchar)convolution[x][i]);
				tmp[x][y].g += (matrix[x][y].g *
					(guchar)convolution[y][x]);
				tmp[x][y].b += (matrix[x][y].b *
					(guchar)convolution[y][x]);
			}
		}
	}

	return tmp;
}

Pixel** UExtract(Pixel **matrix, int matrixSize, int extractSize, int pos_x,
	int pos_y)
{
	Pixel **tmp;
	Pixel defaultPix;
	defaultPix.r = 255;
	defaultPix.g = 255;
	defaultPix.b = 255;
	defaultPix.a = 255;

	tmp = malloc(extractSize * sizeof(Pixel *));
	for (int i = 0; i < extractSize; i++)
	{
		tmp[i] = malloc(extractSize * sizeof(Pixel));
	}
	
	
	for (int y = pos_y - (extractSize/2); y < pos_y + (extractSize/2); y++)
	{
		for (int x = pos_x - (extractSize/2); x < pos_x + (extractSize/2); x++)
		{
			if (x >= 0 && x < matrixSize &&
					y >= 0 && y < matrixSize)
				tmp[x][y] = matrix[x][y];
			else
				tmp[x][y] = defaultPix;
		}
	}

	return tmp;
}

Image UConvolution(Image ref, double **convolution, int size,
	int matrixSize)
{
	Image image;
	Pixel **result;
	Pixel **subMatrix;

	image.width = ref.width;
	image.height = ref.height;
	image.bits_per_sample = ref.bits_per_sample;
	image.has_alpha = ref.has_alpha;

	result = malloc(size * sizeof(Pixel *));
	for (int i = 0; i < size; i++)
	{
		result[i] = malloc(size * sizeof(Pixel));
	}
	
	for (int y = 0; y < matrixSize; y++)
	{
		for (int x = 0; x < matrixSize; x++)
		{
			subMatrix = UExtract(ref.pixList, size, matrixSize, x, y);
			subMatrix = UMultiply(subMatrix, convolution,
				matrixSize);

			result[x][y] = subMatrix
				[matrixSize / 2][matrixSize / 2];
		}
	}

	image.pixList = result;
	return image;
}

Image URotate(Image ref, double angle)
{
	Image image;
	// Calculate Rotation 
	double radian = (angle * M_PI) / 180;
	int newWidth;
	int newHeight;
	Vector2 min, max;
	// Calculate 4 obvious points to find the new size of the image
	Vector2 p[] = {{0,0}, {ref.width, 0},
		{0, ref.height}, {ref.width, ref.height}};
	
	for(int i = 0; i < 4; i++)
	{
		p[i] = ApplyVectorRot(p[i], radian);
	}

	ExtremumVectorValues(p, 4, &min, &max);
	newWidth = max.x - min.x + 1;

	newHeight = max.y - min.y + 1;
	
	image.width = newWidth;
	image.height = newHeight;
	image.bits_per_sample = ref.bits_per_sample;
	image.has_alpha = ref.has_alpha;

	Pixel **pix;
	pix = malloc(newWidth * sizeof(Pixel *));

	for (int i = 0; i < newWidth; i++)
		pix[i] = malloc(newHeight * sizeof(Pixel));

	for (int y = 0; y < newHeight; y++)
		for (int x = 0; x < newWidth; x++)
		{	
			pix[x][y].r = 255;
			pix[x][y].g = 255;
			pix[x][y].b = 255;
			pix[x][y].a = 0;
		}

	for (int y = ref.height - 1; y >= 0; y--)
	{
		for (int x = 0; x < ref.width; x++)
		{
			Vector2 tmp = {x, y};
			tmp = ApplyVectorRot(tmp, radian);

			if ((tmp.x - min.x>= 0 && tmp.x - min.x < newWidth)
				&& (tmp.y - min.y>= 0 && tmp.y - min.y < newHeight))
			{
				pix[tmp.x - min.x][tmp.y - min.y] = ref.pixList[x][y];
				
			}
		}
	}

	image.pixList = pix;
	return image;

}

Vector2 ApplyVectorRot(Vector2 origin, double radian)
{
	/*Vector2 result = {origin.x * cos(radian) + origin.y * sin(radian) + 0.5, 
		-origin.x * sin(radian) + origin.y * cos(radian) + 0.5};*/

	// Shear rotation
	double x1 = round(origin.x - origin.y * tan(radian / 2));
	double y1 = round(sin(radian) * x1 + origin.y);
	double xResult = round(x1 - tan(radian / 2) * y1);
	Vector2 result = {xResult, y1};

	return result;
}

void ExtremumVectorValues(Vector2 *tab, int arraySize, Vector2 *min,
	 Vector2 *max)
{
	*min = tab[0];
	*max = tab[0];
	for(int i = 1; i < arraySize; i++)
	{
		min->x = (min->x <= tab[i].x) ? min->x : tab[i].x;
		min->y = (min->y <= tab[i].y) ? min->y : tab[i].y;
		max->x = (max->x >= tab[i].x) ? max->x : tab[i].x;
		max->y = (max->y >= tab[i].y) ? max->y : tab[i].y;
	}
}
