/* xsyscall_sys.h - indirect system call definitions */

#ifndef LIBF_SL_XSYSCALL_SYS_H
#define LIBF_SL_XSYSCALL_SYS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xsyscall_linux_sys.S */
uint64 XSYMBOL(syscall_linux)(uint64 n, ...);

#ifdef __cplusplus
}
#endif


#endif
