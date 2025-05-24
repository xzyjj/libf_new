/* xpthread_gtp.S - get thread pointer tls (thread local storage) */

#include <libf/config.h>


	.text
	.globl XSYMBOL(pthread_gtp)

/* @func: thread_gtp - get thread pointer
* @return: void * # return thread pointer
*/
XSYMBOL(pthread_gtp):
#ifdef LIBF_MARCH_TYPE
#	if (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_32)

	movl %gs:0, %eax	// ret = $gs:0;
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_64)

	movq %fs:0, %rax	// ret = $fs:0;
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_32)

	mrc p15, 0, r0, c13, c0, 3	// mrc($cp15, $0, ret, $c13, $c0, $3);
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_64)

	mrs x0, tpidr_el0	// msr(ret, $tpidr_el0);
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_32 \
		|| LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_64)

	mv a0, tp	// ret = $tp;
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_32 \
		|| LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_64)

	rdhwr $v0, $gp	// ret = $gp;
	jr $ra

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_IA64_64)
#		error "!!!unimplemente LIBF_MARCH_TYPE_IA64_64!!!"
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_PPC_64)
#		error "!!!unimplemente LIBF_MARCH_TYPE_PPC_64!!!"
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_S390_64)
#		error "!!!unimplemente LIBF_MARCH_TYPE_S390_64!!!"
#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_LOONGARCH_64)
#		error "!!!unimplemente LIBF_MARCH_TYPE_LOONGARCH_64!!!"
#	else
#		error "!!!unknown LIBF_MARCH_TYPE!!!"
#	endif
#else
#	error "!!!undefined LIBF_MARCH_TYPE!!!"
#endif
/* end */
