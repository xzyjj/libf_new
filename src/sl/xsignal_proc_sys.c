/* xsignal_proc_sys.c - standard library signals implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xerrno.h>
#include <libf/sl/xtype_sys.h>
#include <libf/sl/xsyscall_sys.h>
#include <libf/sl/xsignal_sys.h>


/* @func: kill - send a signal to a process or process group
* @param1: pid32_t # process id (-N: group)
* @param2: int32   # signal value
* @return: int32   # 0: no error, -1: errno
*/
int32 XSYMBOL(kill)(pid32_t pid, int32 sig) {
	pid32_t ret = -1;

#ifdef LIBF_PLATFORM_TYPE
#	if (LIBF_PLATFORM_TYPE == LIBF_PLATFORM_TYPE_LINUX)

	ret = (pid32_t)XSYMBOL(syscall_linux)(XSYS_kill, pid, sig);

#	elif (LIBF_PLATFORM_TYPE == LIBF_PLATFORM_TYPE_FREEBSD)
#		error "!!!unimplemente LIBF_PLATFORM_TYPE_FREEBSD!!!"
#	elif (LIBF_PLATFORM_TYPE == LIBF_PLATFORM_TYPE_OPENBSD)
#		error "!!!unimplemente LIBF_PLATFORM_TYPE_OPENBSD!!!"
#	else
#		error "!!!unknown LIBF_PLATFORM_TYPE!!!"
#	endif
#else
#	error "!!!undefined LIBF_PLATFORM_TYPE!!!"
#endif

	if (ret) {
		/* errno */
		return -1;
	}

	return 0;
} /* end */

/* @func: killpg - send a signal to a process group
* @param1: pid32_t # process group id
* @param2: int32   # signal value
* @return: int32   # 0: no error, -1: errno
*/
int32 XSYMBOL(killpg)(pid32_t pgrp, int32 sig) {
	return XSYMBOL(kill)(-pgrp, sig);
} /* end */
