#pragma once

#include <errno.h>
#include <stdio.h>

#include "quicksort.h"
#include "vec.h"

#define ERRNOCHECK(condition, errmsg, label)                           \
	do {                                                           \
		if (condition) {                                       \
			int errsv = errno;                             \
			fprintf(stderr, "%s:%d:%s: Error: ", __FILE__, \
				__LINE__, __func__);                   \
			errno = errsv;                                 \
			perror((errmsg));                              \
			goto label;                                    \
		}                                                      \
	} while (0)

#define ARRLEN(arr) (sizeof(arr) / sizeof(typeof(*arr)))

void swap(int *arr, size_t a, size_t b);
const char *strat_to_str(PivotStrategy s);
const char *type_to_str(VecType t);