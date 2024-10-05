#include <stdlib.h>

#include "util.h"

void swap(int *arr, size_t a, size_t b)
{
	int temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}