#include <stdio.h>
#include <stdlib.h>

#include "cron.h"
#include "quicksort.h"
#include "util.h"
#include "vec.h"

#define CSV_HDR "type,strat,num,time"
#define DEFAULT_MAX_VEC_NUM 10'000
#define DEFAULT_STEP 10

const VecType types[] = { VEC_ORDERED, VEC_REVERSE, VEC_PARTIAL_ORDERED,
			  VEC_RAND };
const PivotStrategy strats[] = { PIVOT_FIRST, PIVOT_LAST, PIVOT_RAND,
				 PIVOT_MEDIAN };

int main(int argc, char **argv)
{
	int max_vec_num = DEFAULT_MAX_VEC_NUM;
	int step = DEFAULT_STEP;
	Vec orig, temp;

	switch (argc) {
	case 1:
		break;
	case 3:
		max_vec_num = atoi(argv[2]);
		[[fallthrough]];
	case 2:
		step = atoi(argv[1]);
		break;
	}

	orig = vec_new(max_vec_num);
	temp = vec_new(max_vec_num);

	puts(CSV_HDR);

	for (size_t i = 0; i < ARRLEN(types); ++i) {
		VecType type = types[i];
		for (int i = 0; i < max_vec_num; i += step) {
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