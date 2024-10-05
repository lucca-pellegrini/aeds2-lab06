#pragma once

#include <stdlib.h>

typedef enum PivotStrategy {
	PIVOT_FIRST,
	PIVOT_LAST,
	PIVOT_RAND,
	PIVOT_MEDIAN
} PivotStrategy;

typedef void sort_func_t(int *, size_t);

void quicksort(int *vec, size_t n, enum PivotStrategy strat);
void quicksort_pivot_first(int *vec, size_t n);
void quicksort_pivot_last(int *vec, size_t n);
void quicksort_pivot_rand(int *vec, size_t n);
void quicksort_pivot_median(int *vec, size_t n);