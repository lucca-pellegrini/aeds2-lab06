#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "vec.h"
#include "util.h"

static bool rand_is_seeded = false;

void vec(Vec *restrict v, size_t n, VecType t)
{
	switch (t) {
	case VEC_ORDERED:
		vec_ordered(v, n);
		break;
	case VEC_REVERSE:
		vec_reverse(v, n);
		break;
	case VEC_PARTIAL_ORDERED:
		vec_partial_ordered(v, n);
		break;
	case VEC_RAND:
		vec_rand(v, n);
		break;
	}
}

void vec_ordered(Vec *restrict v, size_t n)
{
	constexpr int min = 1;

	for (size_t i = 0; i < n; ++i)
		v->arr[i] = min + i;
	v->n = n;
}

void vec_reverse(Vec *restrict v, size_t n)
{
	constexpr int min = 1;

	for (size_t i = n; i > 0; --i)
		v->arr[i - 1] = min + i - 1;
	v->n = n;
}

void vec_partial_ordered(Vec *restrict v, size_t n)
{
	if (!rand_is_seeded) {
		rand_is_seeded = true;
		srand((unsigned)time(NULL));
	}

	// Cria o vetor ordenado.
	vec_ordered(v, n);

	// Embaralha 1 a cada 10 elementos.
	for (size_t i = 0; i < n / 10; ++i)
		swap(v->arr, rand() % n, rand() % n);
	v->n = n;
}

void vec_rand(Vec *restrict v, size_t n)
{
	if (!rand_is_seeded) {
		rand_is_seeded = true;
		srand((unsigned)time(NULL));
	}

	for (size_t i = 0; i < n; ++i)
		v->arr[i] = (rand() % n) + 1;
	v->n = n;
}

Vec vec_new(size_t cap)
{
	int *arr = calloc(cap, sizeof(int));
	ERRNOCHECK(!arr, "Erro ao alocar memória para array.", die);

	return (Vec){ .arr = arr, .n = 0, .cap = cap };

die:
	exit(EXIT_FAILURE);
}

void vec_copy(Vec *restrict dst, Vec src)
{
	ERRNOCHECK(dst->cap < src.n, "Não há espaço no arranjo", die);
	memcpy(dst->arr, src.arr, src.n * sizeof(int));
	dst->n = src.n;
	return;
die:
	exit(EXIT_FAILURE);
}

void vec_free(Vec vec)
{
	free(vec.arr);
}