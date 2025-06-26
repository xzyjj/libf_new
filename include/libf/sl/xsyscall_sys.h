/* xsyscall_sys.h - indirect system call definitions */

#ifndef LIBF_SL_XSYSCALL_SYS_H
#define LIBF_SL_XSYSCALL_SYS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#ifdef LIBF_PLATFORM_TYPE
#	if (LIBF_PLATFORM_TYPE == LIBF_PLATFORM_TYPE_LINUX)
#		include <libf/sl/xsyscall_linux.h>
#	elif (LIBF_PLATFORM_TYPE == LIBF_PLATFORM_TYPE_FREEBSD)
#		include <libf/sl/xsyscall_freebsd.h>
#	elif (LIBF_PLATFORM_TYPE == LIBF_PLATFORM_TYPE_OPENBSD)
#		include <libf/sl/xsyscall_openbsd.h>
#	else
#		error "!!!unknown LIBF_PLATFORM_TYPE!!!"
#	endif
#else
#	error "!!!undefined LIBF_PLATFORM_TYPE!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* xsyscall_linux_sys.S */
uint64 XSYMBOL(syscall_linux)(uint64 n, ...);

#ifdef __cplusplus
}
#endif


#endif
