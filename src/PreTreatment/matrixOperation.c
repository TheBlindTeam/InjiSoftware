#include "matrixOperation.h"

void ClampPixel(Pixel *pix, int min, int max)
{
	pix->r = (pix->r <= min) ? min : pix->r;
	pix->r = (pix->r >= max) ? max : pix->r;

	pix->g = (pix->g <= min) ? min : pix->g;
	pix->g = (pix->g >= max) ? max : pix->g;

	pix->b = (pix->b <= min) ? min : pix->b;
	pix->b = (pix->b >= max) ? max : pix->b;
}

int ClampDouble(double a, int min, int max)
{
	int r = round(a);
	return r < min ? min : (r > max ? max : r);
}

Image *UConvolution(Image *ref, double **convolution, int matrixSize)
{
	Image *image = malloc(sizeof(Image));
	image->width = ref->width;
	image->height = ref->height;
	image->bits_per_sample = ref->bits_per_sample;
	image->has_alpha = ref->has_alpha;
	int half = matrixSize / 2;
	image->pixList = malloc(image->width * sizeof(Pixel*));
	for (int i = 0; i < image->width; i++)
		image->pixList[i] = malloc(image->height * sizeof(Pixel));

	for (int x = 0; x < image->width; x ++)
		for (int y = 0; y < image->height; y ++)
		{
			double R, G, B;
			R = G = B = 0;
			for (int k = -half; k <= half; k ++)
				for (int l = -half; l <= half; l ++)
				{
					if (x + k >= 0 && x + k < image->width &&
						y + l >= 0 && y + l < image->height)
					{
						R += ref->pixList[x + k][y + l].r 
                                                    * convolution[k + half][l + half];
						G += ref->pixList[x + k][y + l].r 
                                                    * convolution[k + half][l + half];
						B += ref->pixList[x + k][y + l].r
                                                    * convolution[k + half][l + half];
					}
				}
			Pixel tmp;
			image->pixList[x][y] = tmp;
			image->pixList[x][y].r = ClampDouble(R, 0, 255);
			image->pixList[x][y].g = ClampDouble(G, 0, 255);
			image->pixList[x][y].b = ClampDouble(B, 0, 255);
			image->pixList[x][y].a = ref->pixList[x][y].a;
		}
	return image;
}

ImageGS *MedianFilter(ImageGS *ref, size_t filterSize)
{
    ImageGS *result = malloc(sizeof(ImageGS));
    // Size of the Filter Matrix
    int arraySize = filterSize;// This value will be updated with ExtractNeighbors
    guchar *neighbors;

    // Image Initialization
    result->width = ref->width;
    result->height = ref->height;
    result->intensity = malloc(result->width * sizeof(guchar *));
    for(int y = 0; y < result->width; y++)
        result->intensity[y] = malloc(result->height * sizeof(guchar));

    for(int y = 0; y < result->height; y++)
        for(int x = 0; x < result->width; x++)
        {
            arraySize = filterSize;
            // Extract the neighbors and get the 1-D array size
            neighbors = ExtractNeighbors(ref, x, y, &arraySize);

            // Sort neighbors' array
            BubbleSort(neighbors, arraySize);

            // Get the median value
            if(arraySize % 2)
            {
                result->intensity[x][y] =  neighbors[arraySize / 2];
           }
            else
            {
                result->intensity[x][y] = (neighbors[arraySize / 2]
                        + neighbors[arraySize / 2 + 1]) / 2;
            }

            // Free previous neighbors allocation
            free(neighbors);
        }

    return result;
}

Image *URotate(Image *ref, double angle)
{
	Image *image = malloc(sizeof(Image));
	// Calculate Rotation 
	double radian = angle;
	int newWidth;
	int newHeight;
	Vector2 min, max;
	// Calculate 4 obvious points to find the new size of the image
	Vector2 p[] = {{0,0}, {ref->width, 0},
		{0, ref->height}, {ref->width, ref->height}};
	
	for(int i = 0; i < 4; i++)
	{
		p[i] = ApplyVectorRot(p[i], radian);
	}

	ExtremumVectorValues(p, 4, &min, &max);
	newWidth = max.x - min.x + 1;

	newHeight = max.y - min.y + 1;
	
	image->width = newWidth;
	image->height = newHeight;
	image->bits_per_sample = ref->bits_per_sample;
	image->has_alpha = ref->has_alpha;

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

	for (int y = ref->height - 1; y >= 0; y--)
	{
		for (int x = 0; x < ref->width; x++)
		{
			Vector2 tmp = {x, y};
			tmp = ApplyVectorRot(tmp, radian);

			if ((tmp.x - min.x>= 0 && tmp.x - min.x < newWidth)
				&& (tmp.y - min.y>= 0
					&& tmp.y - min.y < newHeight))
			{
				pix[tmp.x - min.x][tmp.y - min.y] =
					ref->pixList[x][y];
				
			}
		}
	}

	image->pixList = pix;
	return image;

}

Vector2 ApplyVectorRot(Vector2 origin, double radian)
{
	/*Vector2 result = {origin.x * cos(radian) + origin.y
	 * * sin(radian) + 0.5,
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

guchar *ExtractNeighbors(ImageGS *ref, int posX, int posY, int *size)
{

    int matrixSize = *size;
    guchar *neighbors;

    *size = 0;
    // Find the size of the return's array
    for(int y = (posY - matrixSize / 2); y <= (posY + matrixSize / 2); y++)
        for(int x = (posX - matrixSize / 2); x <= (posX + matrixSize / 2); x++)
        {
            if((x >= 0 && x < ref->width) &&
                    (y >= 0 && y < ref->height))
            {
                (*size)++;
            }
        }
    // Allocate the memory
    neighbors = malloc(*size * sizeof(guchar));

    int index = 0;
    for(int y = (posY - matrixSize / 2); y <= (posY + matrixSize / 2); y++)
        for(int x = (posX - matrixSize / 2); x <= (posX + matrixSize / 2); x++)
        {
            if((y >= 0 && y < ref->height) &&
                    (x >= 0 && x < ref->width))
            {
                neighbors[index] =
                    ref->intensity[x][y]; // Save each neighbors
                index++;
            }
        }

    return neighbors;
}
