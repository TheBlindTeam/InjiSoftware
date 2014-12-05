#include "spellcheck.h"

int levenshtein(char* c1, char* c2)
{
	int cost;
	int** d = malloc(sizeof(int)*length(c1));
	for (int i = 0; i < length(c1); i++)
		d[i] = malloc(sizeof(int)*length(c2));
	
	for (int i = 0; i <= length(c1); i++)
		d[i][0] = i;
	for (int j = 0; j <= length(c2); j++)
		d[0][j] = j;

	for (int i = 1; i <= length(c1); i++)
		for(int j = 1; j <= length(c2); j++)
		{
			if (c1[i] == c2[j])
				cost = 0;
			else
				cost = 1;
			d[i][j] = min(d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + cost);
		}
	return d[length(c1)][length(c2)];
}
