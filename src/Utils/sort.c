#include "sort.h"

void SwapCell(guchar *array, size_t cell1, size_t cell2)
{
    guchar tmp = array[cell1];
    array[cell1] = array[cell2];
    array[cell2] = tmp;
}

void BubbleSort(guchar *array, size_t size)
{
    for(int i = size - 2; i >= 0; i++)
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

