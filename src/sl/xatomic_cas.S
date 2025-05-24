/* xatomic_cas.S - atomic operation implementations */

#include <libf/config.h>


	.text
	.globl XSYMBOL(atomic_cas)

/* @func: atomic_cas - atomic cas (compare-and-swap)
* @param1: int32 * # variable pointer
* @param2: int32   # old value
* @param3: int32   # new value
* @return: int32   # old: success, *var: fail
*/
XSYMBOL(atomic_cas):
#ifdef LIBF_MARCH_TYPE
#	if (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_32)

	pushl %ebx
	movl 8(%esp), %ebx		// $ebx = var;
	movl 12(%esp), %eax		// $eax = old;
	movl 16(%esp), %ecx		// $ecx = new;
	lock cmpxchg %ecx, (%ebx)	// cmpxchg(new, var);
	popl %ebx
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_X86_64)

	movq %rsi, %rax			// $rax = old;
	lock cmpxchg %rdx, (%rdi)	// cmpxchg(new, var);
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_32)

	dmb ish			// # memory access order
.L1:				// do {
	ldrex r3, [r0]		//   ldrex($r3, var);
	cmp r3, r1		//   if ($r3 != old)
	bne .L2			//     goto ret;
	strex r3, r2, [r0]	//   strex($r3, new, var);
	cmp r3, #0		// } while ($r3
	bne .L1			//   );
	mov r3, r1		// $r3 = old;
.L2:				// ret:
	mov r0, r3		// ret = $r3;
	dmb ish			// # memory access order
	bx lr

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_ARM_64)

	dmb ish			// # memory access order
.L1:				// do {
	ldxr w3, [x0]		//   ldrex($w3, var);
	cmp w3, w1		//   if ($w3 != old)
	bne .L2			//     goto ret;
	stxr w3, w2, [x0]	//   stxr($w3, new, var);
	cbnz w3, .L1		// } while ($w3);
	mov w3, w1		// $w3 = old;
.L2:				// ret:
	mov w0, w3		// ret = $w3;
	dmb ish			// # memory access order
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_32 \
		|| LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_RISCV_64)

	fence rw, rw		// # memory access order
.L1:				// do {
	lr.w.aqrl a3, (a0)	//   lr($a3, var);
	bne a3, a1, .L2		//   if ($a3 != old) goto ret;
	sc.w.aqrl a3, a2, (a0)	//   sc($a3, new, var);
	bnez a3, .L1		// } while ($a3);
	mv a3, a1		// $a3 = old;
.L2:				// ret: 
	mv a0, a3		// ret = $a3;
	fence rw, rw		// # memory access order
	ret

#	elif (LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_32 \
		|| LIBF_MARCH_TYPE == LIBF_MARCH_TYPE_MIPS_64)

	sync			// # memory access order
.L1:				// do {
	ll $v0, ($a0)		//   ll(ret, var);
	bne $v0, $a1, .L2	//   if (ret != old) goto ret;
	move $v0, $a2		//   ret = new;
	sc $v0, ($a0)		//   sc(ret, var);
	beqz $v0, .L1		// } while (!ret);
	move $v0, $a1		// ret = old;
.L2:				// ret:
	sync			// # memory access order
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
