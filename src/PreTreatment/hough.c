#include "hough.h"

double FindInclinationAngle(ImageBN ref)
{
	// This var get back the maximum number value index from ACCU
	int maxAngleAccu = 0;
	int thetaMax = 2 * 90 + 1;
	int rMax = (int)((ref.width * cos(M_PI / 4) + ref.height * sin(M_PI / 4))
		* 2 + 1);
	int **accu = malloc(thetaMax * sizeof(int *));

	for(int i = 0; i < thetaMax; i++)
		accu[i] = malloc(rMax * sizeof(int));

	// Initialize the accumulator
	for (int theta = 0; theta < thetaMax; theta++)
		for (int r = 0; r < rMax; r++)
			accu[theta][r] = 0;

	printf("RMAX is equal to %i\n", rMax);

	for (int y = 0; y < ref.height; y++)
	{
		for (int x = 0; x < ref.width; x++)
		{
			// The pixel is black
			if(ref.data[x][y] == 0)
			{
				for (int theta = -90; theta < 90; theta++)
				{
					double radian = (M_PI * theta) / 180;
					int r = (int)(x * cos(radian) + y * sin(radian));
					r = (r <= 0) ? r + rMax : r;
					// Increase the accumulator
					int thetaPos = theta + (thetaMax / 2);
					accu[thetaPos][r]++;
				}
			}
		}
	}

	maxAngleAccu = GetMaxIndex(accu, thetaMax, rMax);

	for(int i = 0; i < thetaMax; i++)
		free(accu[i]);
	free(accu);

	return ((maxAngleAccu * M_PI) / 180);
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
