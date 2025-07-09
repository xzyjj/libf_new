= linux uapi header

== C-flags

	> 'include' is replaced according to the actual path

	x86_32:

	$ -U__i386__ -D__i386__ -Iinclude -Iinclude/asm-x86

	x86_64:

	$ -U__i386__ -U__ILP32__ -Iinclude -Iinclude/asm-x86

	arm_32:

	$ -U__ARM_EABI__ -D__ARM_EABI__ -Iinclude -Iinclude/asm-arm

	arm_64:

	$ -Iinclude -Iinclude/asm-arm64

	riscv_32:

	$ -U__LP64__ -Iinclude -Iinclude/asm-riscv

	riscv_64:

	$ -U__LP64__ -D__LP64__ -Iinclude -Iinclude/asm-riscv

	mips_32:

	$ -U_MIPS_SIM -D_MIPS_SIM=2 -Iinclude -Iinclude/asm-mips
	
	misc_64:

	$ -U_MIPS_SIM -D_MIPS_SIM=3 -Iinclude -Iinclude/asm-mips

	ia64:

	$ -Iinclude -Iinclude/asm-ia64

	ppc_64:

	$ -U__powerpc64__ -D__powerpc64__ -Iinclude -Iinclude/asm-powerpc

	s390_64:

	$ -U__s390x__ -D__s390x__ -Iinclude -Iinclude/asm-s390

	loongarch_64:

	$ -Iinclude -Iinclude/asm-loongarch
