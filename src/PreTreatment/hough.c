#include "hough.h"

double FindInclinationAngle(Image ref)
{
	int **accu;
	int thetaMax = 2 * 90 + 1;
	int rMax = (int)((ref.width * cos(M_PI / 4) + ref.height * sin(M_PI / 4))
		* 2 + 1);
	accu = malloc(thetaMax * sizeof(int *));

	for(int i = 0; i < thetaMax; i++)
	{
		accu[i] = malloc(rMax * sizeof(int));
	}

	// Initialize the accumulator
	for (int theta = 0; theta < thetaMax; theta++)
	{
		for (int r = 0; r < rMax; r++)
		{
			accu[theta][r] = 0;
		}
	}

	for (int y = 0; y < ref.height; y++)
	{
		for (int x = 0; x < ref.width; x++)
		{
			for (int theta = - 90; theta < 90; theta++)
			{
				double radian = (M_PI * theta) / 180;
				int r = (int)(x * cos(radian) + y * sin(radian));
				r = (r <= 0) ? r + rMax : r;

				// Increase the accumulator
				accu[(theta <= 0) ? theta + thetaMax : theta][r]++;
			}
		}
	}

	return ((GetMaxIndex(accu,
			ref.width, ref.height) * M_PI) / 180);
}

int GetMaxIndex(int **array, int sizeX, int sizeY)
{
	int maxVal = array[0][0];
	int xMaxIndex = 0;
	for(int y = 0; y < sizeY; y++)
	{
		for(int x = 0; x < sizeX; x++)
		{
			if(maxVal <= array[x][y])
			{
				maxVal = array[x][y];
				xMaxIndex = x;
			}
		}
	}

	return xMaxIndex;
}
