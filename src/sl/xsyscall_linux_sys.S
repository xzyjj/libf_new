/* xsyscall_linux_sys.c - linux indirect system call implementations */

#include <libf/config.h>


	.text
	.globl XSYMBOL(syscall_linux)

/* @func: syscall_linux - indirect system call
* @param1: uint64 # syscall number
* @param2: uint64 # syscall arg1
* @param3: uint64 # syscall arg2
* @param4: uint64 # syscall arg3
* @param5: uint64 # syscall arg4
* @param6: uint64 # syscall arg5
* @param7: uint64 # syscall arg6
* @return: uint64 # syscall return
*/
XSYMBOL(syscall_linux):
#ifdef LIBF_MARCH_TYPE
#	if (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_32)

	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 20(%esp), %eax /* syscall number */
	movl 24(%esp), %ebx
	movl 28(%esp), %ecx
	movl 32(%esp), %edx
	movl 36(%esp), %esi
	movl 40(%esp), %edi
	movl 44(%esp), %ebp
	int $0x80
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_64)

	movq %rdi, %rax /* syscall number */
	movq %rsi, %rdi
	movq %rdx, %rsi
	movq %rcx, %rdx
	movq %r8, %r10
	movq %r9, %r8
	movq 8(%rsp), %r9
	syscall
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_32)

	mov ip, sp
	push { r4, r5, r6, r7 } /* callee */
	mov r7, r0 /* syscall number */
	mov r0, r1
	mov r1, r2
	mov r2, r3
	ldmfd ip, { r3, r4, r5, r6 /* arg7 */ }
	swi #0
	pop { r4, r5, r6, r7 }
	bx lr

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_64)

	mov x8, x0 /* syscall number */
	mov x0, x1
	mov x1, x2
	mov x2, x3
	mov x3, x4
	mov x4, x5
	mov x5, x6
	mov x6, x7 /* arg7 */
	svc #0
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_32 \
		|| LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_64)

	mv a7, a0 /* syscall number */
	mv a0, a1
	mv a1, a2
	mv a2, a3
	mv a3, a4
	mv a4, a5
	mv a5, a6
	ecall
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_32)

	move $v0, $a0 /* syscall number */
	move $a0, $a1
	move $a1, $a2
	move $a2, $a3
	lw $a3, 4($sp)
	lw $t0, 8($sp)
	lw $t1, 12($sp)
	lw $t2, 16($sp)
	syscall
	jr $ra

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_64)

	move $v0, $a0 /* syscall number */
	move $a0, $a1
	move $a1, $a2
	move $a2, $a3
	move $a3, $a4
	move $a4, $a5
	move $a5, $a6
	move $a6, $a7 /* arg7 */
	syscall
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
