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


/* @def: linux */
#undef XSYS_kill
#define XSYS_kill __NR_kill
/* end */


#endif
