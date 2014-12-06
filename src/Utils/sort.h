#ifndef SORT_H
#define SORT_H

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

static inline
void SwapCell(guchar *array, int cell1, int cell2)
{
    guchar tmp = array[cell1];
    array[cell1] = array[cell2];
    array[cell2] = tmp;
}

void BubbleSort(guchar *array, int size);
#endif
