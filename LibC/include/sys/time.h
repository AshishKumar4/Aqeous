#ifndef _TIME_H_
#define _TIME_H_

typedef uint32_t time_t;
typedef uint32_t suseconds_t;

struct timeval 
{
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* microseconds */
};

struct timezone 
{
    int tz_minuteswest;     /* minutes west of Greenwich */
    int tz_dsttime;         /* type of DST correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz); 
int settimeofday(const struct timeval *tv , const struct timezone *tz);

time_t time(time_t* t);

#endif 