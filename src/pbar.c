#include <stdio.h>

void progress_bar(unsigned current, unsigned total)
{
	constexpr int bar_width = 60;
	double percentage = (double)current / total;

	/* We use eighths of blocks from the Symbols for Legacy Computing
	 * unicode block. This allows for a very granular progress bar, so we
	 * do everything in fractions of 1/8. */
	int num_eighth_blocks = (int)(percentage * bar_width * 8);
	int num_full_blocks = num_eighth_blocks / 8;

	fputs("\r\342\224\244", stderr);
	for (int i = 0; i < bar_width; ++i) {
		if (i < num_full_blocks) {
			fputs("\u2588", stderr);
		} else if (i == num_full_blocks) {
			switch (num_eighth_blocks % 8) {
			case 0:
				fputs(" ", stderr);
				break;
			case 1:
				fputs("\u258F", stderr);
				break;
			case 2:
				fputs("\u258E", stderr);
				break;
			case 3:
				fputs("\u258D", stderr);
				break;
			case 4:
				fputs("\u258C", stderr);
				break;
			case 5:
				fputs("\u258B", stderr);
				break;
			case 6:
				fputs("\u258A", stderr);
				break;
			case 7:
				fputs("\u2589", stderr);
				break;
			}
		} else {
			fputs(" ", stderr);
		}
	}
	fprintf(stderr, "\342\224\234 %.2lf%% (%u/%u)", percentage * 100,
		current, total);
}

void hide_cursor(void)
{
	fputs("\033[?25l\n", stderr);
}

void show_cursor(void)
{
	fputs("\033[?25h\n", stderr);
}