#include "sort.h"

void BubbleSort(guchar *array, int size)
{
    for(int i = size - 1; i >= 0; i--)
    {
        int toSwap = 1;
        for(int j = 0; j < i; j++)
        {
            if(array[j] > array[j+1])
            {
                SwapCell(array, j, (j+1));
                toSwap = 0;
            }
        }

        if(toSwap)
            return;
    }
}


