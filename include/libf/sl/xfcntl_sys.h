/* xfcntl_sys.h - definitions */

#ifndef LIBF_SL_XFCNTL_SYS_H
#define LIBF_SL_XFCNTL_SYS_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: fcntl */
#undef XSYS_F_DUPFD
#define XSYS_F_DUPFD 0
#undef XSYS_F_GETFD
#define XSYS_F_GETFD 1
#undef XSYS_F_SETFD
#define XSYS_F_SETFD 2
#undef XSYS_F_GETFL
#define XSYS_F_GETFL 3
#undef XSYS_F_SETFL
#define XSYS_F_SETFL 4

#undef XSYS_F_GETLK
#define XSYS_F_GETLK 5
#undef XSYS_F_SETLK
#define XSYS_F_SETLK 6
#undef XSYS_F_SETLKW
#define XSYS_F_SETLKW 7

#undef XSYS_F_GETOWN
#define XSYS_F_GETOWN 8
#undef XSYS_F_SETOWN
#define XSYS_F_SETOWN 9

#undef XSYS_F_DUPFD_CLOEXEC
#define XSYS_F_DUPFD_CLOEXEC 10
/* end */


#endif
