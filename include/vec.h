#pragma once

#include <stdlib.h>

typedef struct Vec {
	int *arr;
	size_t n;
	size_t cap;
} Vec;

typedef enum VecType {
	VEC_ORDERED,
	VEC_REVERSE,
	VEC_PARTIAL_ORDERED,
	VEC_RAND
} VecType;

void vec(Vec *restrict v, size_t n, VecType t);
void vec_ordered(Vec *restrict v, size_t n);
void vec_reverse(Vec *restrict v, size_t n);
void vec_partial_ordered(Vec *restrict v, size_t n);
void vec_rand(Vec *restrict v, size_t n);
Vec vec_new(size_t cap);
void vec_copy(Vec *restrict dst, Vec src);
void vec_free(Vec vec);