/* xsyscall_linux.h - linux syscall number definitions */

#ifndef LIBF_SL_XSYSCALL_LINUX_H
#define LIBF_SL_XSYSCALL_LINUX_H

#include <libf/config.h>
#ifdef LIBF_MARCH_TYPE
#	if (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_32)
#		undef __i386__
#		define __i386__
#		include <asm/unistd.h>
#		undef __i386__
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_64)
#		undef __i386__
#		undef __ILP32__
#		include <asm/unistd.h>
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_32)
#		undef __ARM_EABI__
#		define __ARM_EABI__
#		include <asm/unistd.h>
#		undef __ARM_EABI__
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_64)
#		include <asm/unistd.h>
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_32)
#		undef __LP64__
#		include <asm/unistd.h>
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_64)
#		undef __LP64__
#		define __LP64__
#		include <asm/unistd.h>
#		undef __LP64__
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_32)
#		include <asm/sgidefs.h>
#		undef _MIPS_SIM
#		define _MIPS_SIM _MIPS_SIM_NABI32
#		include <asm/unistd.h>
#		undef _MIPS_SIM
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_64)
#		include <asm/sgidefs.h>
#		undef _MIPS_SIM
#		define _MIPS_SIM _MIPS_SIM_ABI64
#		include <asm/unistd.h>
#		undef _MIPS_SIM
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_IA64_64)
#		include <asm/unistd.h>
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_PPC_64)
#		undef __powerpc64__
#		define __powerpc64__
#		include <asm/unistd.h>
#		undef __powerpc64__
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_S390_64)
#		undef __s390x__
#		define __s390x__
#		include <asm/unistd.h>
#		undef __s390x__
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_LOONGARCH_64)
#		include <asm/unistd.h>
#	else
#		error "!!!unknown LIBF_MARCH_TYPE!!!"
#	endif
#else
#	error "!!!undefined LIBF_MARCH_TYPE!!!"
#endif


/* @def: linux
* unistd */
#undef XSYS_exit
#define XSYS_exit (__NR_exit)
#undef XSYS_clone
#define XSYS_clone (__NR_clone)
#undef XSYS_execve
#define XSYS_execve (__NR_execve)

/* mman */
#undef XSYS_mmap
#define XSYS_mmap (__NR_mmap)
#undef XSYS_munmap
#define XSYS_munmap (__NR_munmap)
#undef XSYS_mlock
#define XSYS_mlock (__NR_mlock)
#undef XSYS_munlock
#define XSYS_munlock (__NR_munlock)
#undef XSYS_mlockall
#define XSYS_mlockall (__NR_mlockall)
#undef XSYS_munlockall
#define XSYS_munlockall (__NR_munlockall)
#undef XSYS_mprotect
#define XSYS_mprotect (__NR_mprotect)
#undef XSYS_msync
#define XSYS_msync (__NR_msync)

/* shm */
#undef XSYS_shmget
#define XSYS_shmget (__NR_shmget)
#undef XSYS_shmat
#define XSYS_shmat (__NR_shmat)
#undef XSYS_shmdt
#define XSYS_shmdt (__NR_shmdt)
#undef XSYS_shmctl
#define XSYS_shmctl (__NR_shmctl)

/* signal */
#undef XSYS_kill
#define XSYS_kill (__NR_kill)
#undef XSYS_tkill
#define XSYS_tkill (__NR_tkill)
#undef XSYS_sigaltstack
#define XSYS_sigaltstack (__NR_sigaltstack)
#undef XSYS_sigaction
#define XSYS_sigaction (__NR_rt_sigaction)
#undef XSYS_sigprocmask
#define XSYS_sigprocmask (__NR_rt_sigprocmask)
#undef XSYS_sigtimedwait
#define XSYS_sigtimedwait (__NR_rt_sigtimedwait)
#undef XSYS_sigreturn
#define XSYS_sigreturn (__NR_rt_sigreturn)
/* end */


#endif
