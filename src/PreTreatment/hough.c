#include "hough.h"

double FindInclinationAngle(ImageBN *ref)
{
	// This var get back the maximum number value index from ACCU
	int maxAngleAccu = 0;
	const int thetaMax = 900;
	const int rMax = round(sqrt(pow(ref->width, 2) +
		pow(ref->height, 2)) * 2 + 1);

	int **accu = malloc(thetaMax * sizeof(int *));

	for(int i = 0; i < thetaMax; i++)
		accu[i] = malloc(rMax * sizeof(int));

	// Initialize the accumulator
	for (int theta = 0; theta < thetaMax; theta++)
		for (int r = 0; r < rMax; r++)
			accu[theta][r] = 0;

	for (int y = 0; y < ref->height; y++)
	{
		for (int x = 0; x < ref->width; x++)
		{
			// The pixel is black
			if(ref->data[x][y] == 0)
			{
				for (int theta = 0; theta < thetaMax; theta++)
				{
					double radian = (M_PI * (double)(theta - thetaMax / 2))
						/ (double)thetaMax;
					int r = round(x * cos(radian) + y * sin(radian));

					r+= rMax / 2;
					// Increase the accumulator
					accu[theta][r]++;
				}
			}
		}
	}

	maxAngleAccu = GetMaxIndex(accu, thetaMax, rMax);
	for(int i = 0; i < thetaMax; i++)
		free(accu[i]);
	free(accu);

	double answer = M_PI * (double)(thetaMax - maxAngleAccu - 1) / (double)thetaMax;
	if (answer > M_PI / 2)
		answer -= M_PI;
	return answer;
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
