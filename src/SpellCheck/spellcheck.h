#include <stdio.h>
#include <stdlib.h>

static inline
int length(char* c)
{
	int i = 0;
	while(c[i])
		i++;
	return i;
}

static inline
int min(int a, int b, int c)
{
	int min;
	if (a < b)
		min = a;
	else
		min = b;
	if (min < c)
		return min;
	else
		return c;
}

void writeFilter(FILE* );

int levenshtein(char* c1, char* c2);
