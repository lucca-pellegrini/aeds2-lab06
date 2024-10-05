#include <stdio.h>
#include <stdlib.h>

#include "quicksort.h"
#include "util.h"
#include "vec.h"

void swap(int *arr, size_t a, size_t b)
{
	int temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

const char *strat_to_str(PivotStrategy s)
{
	switch (s) {
	case PIVOT_FIRST:
		return "FIRST";
	case PIVOT_LAST:
		return "LAST";
	case PIVOT_RAND:
		return "RAND";
	case PIVOT_MEDIAN:
		return "MEDIAN";
	}

	fprintf(stderr, "Estratégia desconhecida: %d\n", s);
	exit(EXIT_FAILURE);
}

const char *type_to_str(VecType t)
{
	switch (t) {
	case VEC_ORDERED:
		return "ORDERED";
	case VEC_REVERSE:
		return "REVERSE";
	case VEC_PARTIAL_ORDERED:
		return "PARTIAL_ORDERED";
	case VEC_RAND:
		return "RAND";
	}

	fprintf(stderr, "Tipo de vetor desconhecido: %d\n", t);
	exit(EXIT_FAILURE);
}