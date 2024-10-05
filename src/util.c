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
		return "Primeiro";
	case PIVOT_LAST:
		return "Último";
	case PIVOT_RAND:
		return "Aleatório";
	case PIVOT_MEDIAN:
		return "Média de Três";
	}

	fprintf(stderr, "Estratégia desconhecida: %d\n", s);
	exit(EXIT_FAILURE);
}

const char *type_to_str(VecType t)
{
	switch (t) {
	case VEC_ORDERED:
		return "Ordenados";
	case VEC_REVERSE:
		return "Ordenados Reversos";
	case VEC_PARTIAL_ORDERED:
		return "Parcialmente Ordenados";
	case VEC_RAND:
		return "Aleatórios";
	}

	fprintf(stderr, "Tipo de vetor desconhecido: %d\n", t);
	exit(EXIT_FAILURE);
}