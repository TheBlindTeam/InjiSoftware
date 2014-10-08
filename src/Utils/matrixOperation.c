#include "matrixOperation.h"

Pixel** UMultiply(**Pixel matrix, **double convolution, int matrixSize)
{
	Pixel tmp[matrixSize][matrixSize];

	for(int y = 0; y < matrixSize; y++)
	{
		for(int x = 0; x < matrix.Size; x++)
		{
			tmp[x][y] += matrix[x][y].r *
				(guchar)convolution[y][x];
			tmp[x][y] += matrix[x][y].g *
				(guchar)convolution[y][x];
			tmp[x][y] += matrix[x][y].b *
				(guchar)convolution[y][x];
		}
	}

	return tmp;
}

Pixel** UExtract(**Pixel matrix, int matrixSize, int size, int pos_x,
	int pos_y)
{
	Pixel tmp[size][size];
	Pixel defaultPix;
	defaultPix.r = 255;
	defaultPix.g = 255;
	defaultPix.b = 255;
	defaultPix.a = 255;
	
	for(int y = pos_y; y < pos_y + size; y++)
	{
		for(int x = pos_x; x < pos_x + size; x++)
		{
			if(x >= 0 && x < matrixSize &&
					y >= 0 && y < matrixSize)
				tmp[x][y] = matrix[x][y];
			else
				tmp[x][y] = defaultPix;
		}
	}

	return tmp;
}

Pixel** UConvolution(**Pixel matrix, **double convolution, int size, 
	int matrixSize)
{
	Pixel result[size][size];
	Pixel subMatrix[matrixSize][matrixSize];
	
	for(int y = 0; y < matrixSize; y++)
	{
		for(int x = 0; x < matrixSize; x++)
		{
			subMatrix = UExtract(matrix, size, matrixSize, x, y);
			subMatrix = UMultiply(subMatrix, convolution,
				matrixSize);

			result[x][y] = subMatrix
				[matrixSize / 2][matrixSize / 2];
		}
	}

	return result;
}
