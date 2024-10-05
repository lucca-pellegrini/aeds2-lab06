#include <stdio.h>
#include <stdlib.h>

#include "cron.h"
#include "quicksort.h"
#include "util.h"
#include "vec.h"

#define CSV_HDR "type,strat,num,time"
#define MAX_VEC_NUM 10'000

const VecType types[] = { VEC_ORDERED, VEC_REVERSE, VEC_PARTIAL_ORDERED,
			  VEC_RAND };
const PivotStrategy strats[] = { PIVOT_FIRST, PIVOT_LAST, PIVOT_RAND,
				 PIVOT_MEDIAN };

int main(void)
{
	Vec orig = vec_new(MAX_VEC_NUM);
	Vec temp = vec_new(MAX_VEC_NUM);

	for (size_t i = 0; i < ARRLEN(types); ++i) {
		VecType type = types[i];
		for (int i = 0; i < MAX_VEC_NUM; ++i) {
			vec(&orig, i, type);
			for (size_t i = 0; i < ARRLEN(strats); ++i) {
				unsigned long time;
				PivotStrategy strat = strats[i];

				vec_copy(&temp, orig);

				cron_start();
				quicksort(temp.arr, temp.n, strat);
				time = cron_stop();

				printf("%s,%s,%ju,%lu\n", type_to_str(type),
				       strat_to_str(strat), temp.n, time);
			}
		}
	}

	vec_free(orig);
	vec_free(temp);
	return EXIT_SUCCESS;
}