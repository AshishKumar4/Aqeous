#include "sys/time.h"


int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    return 0;
}

int settimeofday(const struct timeval *tv , const struct timezone *tz)
{
    return 0;
}

time_t time(time_t* t)
{
    return 0;
}