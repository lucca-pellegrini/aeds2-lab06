#include <stdio.h>
#include <stdlib.h>

#include "cron.h"
#include "pbar.h"
#include "quicksort.h"
#include "util.h"
#include "vec.h"

#define CSV_HDR "type,strat,num,time"

const VecType types[] = { VEC_ORDERED, VEC_REVERSE, VEC_PARTIAL_ORDERED,
			  VEC_RAND };
const PivotStrategy strats[] = { PIVOT_FIRST, PIVOT_LAST, PIVOT_RAND,
				 PIVOT_MEDIAN };

int main(int argc, char **argv)
{
	// Valores padrão.
	constexpr int default_max_vec_num = 10'000;
	constexpr int default_step = 10;

	int total_iterations, num_steps, current_iteration = 0;
	int max_vec_num = default_max_vec_num;
	int step;
	Vec orig, temp;

	switch (argc) {
	case 1:
		step = default_step;
		break;
	case 3:
		max_vec_num = atoi(argv[2]);
		[[fallthrough]];
	case 2:
		step = atoi(argv[1]);
		break;
	default:
		fprintf(stderr, "Usage: %s [STEP [MAX_VEC_NUM]]\n", *argv);
		exit(EXIT_FAILURE);
	}

	num_steps = (max_vec_num + step - 1) / step;
	total_iterations = num_steps * ARRLEN(types) * ARRLEN(strats);

	orig = vec_new(max_vec_num);
	temp = vec_new(max_vec_num);

	hide_cursor();
	progress_bar(current_iteration, total_iterations);

	puts(CSV_HDR);

	for (size_t i = 0; i < ARRLEN(types); ++i) {
		VecType type = types[i];
		for (int j = 1; j <= max_vec_num; j += step) {
			vec(&orig, j, type);
			for (size_t k = 0; k < ARRLEN(strats); ++k) {
				unsigned long time;
				PivotStrategy strat = strats[k];

				vec_copy(&temp, orig);

				cron_start();
				quicksort(temp.arr, temp.n, strat);
				time = cron_stop();

				printf("%s,%s,%ju,%lu\n", type_to_str(type),
				       strat_to_str(strat), temp.n, time);

				progress_bar(++current_iteration,
					     total_iterations);
			}
		}
	}

	progress_bar(current_iteration, total_iterations);
	show_cursor();
	vec_free(orig);
	vec_free(temp);
	return EXIT_SUCCESS;
}