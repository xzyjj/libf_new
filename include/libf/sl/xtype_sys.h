/* xtype_sys.h - posix data type definitions */

#ifndef LIBF_SL_XTYPE_SYS_H
#define LIBF_SL_XTYPE_SYS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: type
* used for device ids */
#undef dev64_t
typedef uint64 dev64_t;
#undef dev64L_t
typedef uint64L dev64L_t;

/* used for user ids */
#undef uid32_t
typedef uint32 uid32_t;

/* used for group ids */
#undef gid32_t
typedef uint32 gid32_t;

/* used for file serial numbers */
#undef ino64_t
typedef uint64 ino64_t;
#undef ino64L_t
typedef uint64L ino64L_t;

/* used for some file attributes */
#undef mode32_t
typedef uint32 mode32_t;

/* used for link counts */
#undef nlink32_t
typedef uint32 nlink32_t;

/* used for file sizes */
#undef off64_t
typedef int64 off64_t;
#undef off64L_t
typedef int64L off64L_t;

/* used as a general identifier */
#undef id32_t
typedef uint32 id32_t;

/* used for process ids */
#undef pid32_t
typedef int32 pid32_t;

/* used for xsi interprocess communication */
#undef key32_t
typedef int32 key32_t;

/* used for block sizes */
#undef blksize64_t
typedef int64 blksize64_t;

/* used for file block counts */
#undef blkcnt64_t
typedef int64 blkcnt64_t;
#undef blkcnt64L_t
typedef int64L blkcnt64L_t;

/* used for file system block counts */
#undef fsblkcnt64_t
typedef uint64 fsblkcnt64_t;
#undef fsblkcnt64L_t
typedef uint64L fsblkcnt64L_t;

/* used for file system file counts */
#undef fsfilcnt64_t
typedef uint64 fsfilcnt64_t;
#undef fsfilcnt64L_t
typedef uint64L fsfilcnt64L_t;
/* end */

/* @def: time
* used for time in seconds */
#undef time32_t
typedef int32 time32_t;
#undef time64_t
typedef int64 time64_t;

/* used for time in microseconds (unsigned) */
#undef useconds32_t
typedef uint32 useconds32_t;

/* used for time in microseconds */
#undef suseconds64_t
typedef int64 suseconds64_t;
#undef suseconds64L_t
typedef int64L suseconds64L_t;

/* used for system times in clock ticks */
#undef clock64_t
typedef int64 clock64_t;

/* used for clock id type in the clock and timer functions */
#undef clockid32_t
typedef int32 clockid32_t;

/* used for timer id returned by timer_create() */
#undef timer64_t
typedef void * timer64_t;
/* end */


#endif
