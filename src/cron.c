#define _POSIX_C_SOURCE 199309L

#include <time.h>

#include "cron.h"

static struct timespec ts = { 0 };

void cron_start(void)
{
	clock_gettime(CLOCK_MONOTONIC, &ts);
}

unsigned long cron_stop(void)
{
	struct timespec end = { 0 };
	clock_gettime(CLOCK_MONOTONIC, &end);
	return (end.tv_sec - ts.tv_sec) * 1'000'000'000UL + (end.tv_nsec - ts.tv_nsec);
}