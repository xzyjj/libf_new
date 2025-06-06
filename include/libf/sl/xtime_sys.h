/* xtime_sys.h - posix time type definitions */

#ifndef LIBF_SL_XTIME_SYS_H
#define LIBF_SL_XTIME_SYS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: time
* time struct in seconds and nanoseconds */
#undef timespec_t
struct timespec_t {
	time64_t tv_sec; /* seconds */
	time64_t tv_nsec; /* nanoseconds */
};

/* time struct in seconds and microseconds */
#undef timeval_t
struct timeval_t {
	time64_t tv_sec; /* seconds */
	suseconds64_t tv_usec; /* microseconds */
};

/* interval for a timer with nanosecond precision */
#undef itimerspec_t
struct itimerspec_t {
	struct timespec_t it_interval;
	struct timespec_t it_value;
};
/* end */


#endif
