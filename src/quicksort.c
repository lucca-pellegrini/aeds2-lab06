#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "quicksort.h"
#include "util.h"

typedef size_t pivot_func_t(int *, size_t, size_t);

static void quicksort_rec(int *vec, size_t esq, size_t dir,
			  pivot_func_t *pivot_func);
static size_t pivot_first(int *vec, size_t esq, size_t dir);
static size_t pivot_last(int *vec, size_t esq, size_t dir);
static size_t pivot_rand(int *vec, size_t esq, size_t dir);
static size_t pivot_median(int *vec, size_t esq, size_t dir);

static bool rand_is_seeded = false;

// Ponto de entrada do quicksort para evitar passar `esq` e `dir` em main.
void quicksort(int *vec, size_t n, PivotStrategy strat)
{
	pivot_func_t *func;

	if (n < 1)
		return;

	switch (strat) {
	case PIVOT_FIRST:
		func = (pivot_func_t *)&pivot_first;
		break;
	case PIVOT_LAST:
		func = (pivot_func_t *)&pivot_last;
		break;
	case PIVOT_RAND:
		func = (pivot_func_t *)&pivot_rand;
		break;
	case PIVOT_MEDIAN:
		func = (pivot_func_t *)&pivot_median;
		break;
	}

	quicksort_rec(vec, 0, n - 1, func);
}

// Função auxiliar que faz o quicksort recursivamente.
static void quicksort_rec(int *vec, size_t esq, size_t dir,
			  pivot_func_t *pivot_func)
{
	size_t i = esq, j = dir;
	int pivo = vec[pivot_func(vec, esq, dir)];

	// Verificação adicional para underflow de `j`, que é unsigned.
	while (i <= j && j != SIZE_MAX) {
		while (vec[i] < pivo)
			++i;
		while (j > 0 && vec[j] > pivo)
			--j;
		if (i <= j)
			swap(vec, i++, j--);
	}

	if (esq < j && j != SIZE_MAX)
		quicksort_rec(vec, esq, j, pivot_func);
	if (i < dir)
		quicksort_rec(vec, i, dir, pivot_func);
}

static size_t pivot_first(int *vec, size_t esq, size_t dir)
{
	(void)vec;
	(void)dir;
	return esq;
}

static size_t pivot_last(int *vec, size_t esq, size_t dir)
{
	(void)vec;
	(void)esq;
	return dir;
}

static size_t pivot_rand(int *vec, size_t esq, size_t dir)
{
	size_t range = dir - esq + 1;
	size_t res;
	(void)vec;

	if (!rand_is_seeded) {
		rand_is_seeded = true;
		srand((unsigned)time(NULL));
	}

	// Amostragem por rejeição para garantir distribuição uniforme.
	do
		res = (unsigned)rand();
	while (res >= (unsigned)RAND_MAX - ((unsigned)RAND_MAX % range));

	return esq + (res % range);
}

static size_t pivot_median(int *restrict vec, size_t esq, size_t dir)
{
	size_t mid = esq + (dir - esq) / 2; // Acha meio sem risco de overflow.

	if (vec[esq] > vec[mid])
		swap(vec, dir, mid);
	if (vec[esq] > vec[dir])
		swap(vec, esq, dir);
	if (vec[mid] > vec[dir])
		swap(vec, mid, dir);

	return mid;
}

void quicksort_pivot_first(int *vec, size_t n)
{
	quicksort(vec, n, PIVOT_FIRST);
}

void quicksort_pivot_last(int *vec, size_t n)
{
	quicksort(vec, n, PIVOT_LAST);
}

void quicksort_pivot_rand(int *vec, size_t n)
{
	quicksort(vec, n, PIVOT_RAND);
}

void quicksort_pivot_median(int *vec, size_t n)
{
	quicksort(vec, n, PIVOT_MEDIAN);
}