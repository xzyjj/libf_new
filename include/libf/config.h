/* config.h - libf config definitions */

#ifndef LIBF_CONFIG_H
#define LIBF_CONFIG_H


/* extern symbol */
#undef FSYMBOL
#define FSYMBOL(name) libf_##name

/* standard extern symbol */
#undef XSYMBOL
#define XSYMBOL(name) x##name

/* machine 32bit/64bit */
#undef LIBF_MARCH_BIT_32
#define LIBF_MARCH_BIT_32 32
#undef LIBF_MARCH_BIT_64
#define LIBF_MARCH_BIT_64 64

/* machine architecture 32bit/64bit */
#undef LIBF_MARCH_BIT
#ifdef __SIZEOF_POINTER__
#	if (__SIZEOF_POINTER__ == 4)
#		define LIBF_MARCH_BIT LIBF_MARCH_BIT_32
#	elif (__SIZEOF_POINTER__ == 8)
#		define LIBF_MARCH_BIT LIBF_MARCH_BIT_64
#	else
#		error "!!!unknown __SIZEOF_POINTER__!!!"
#	endif
#else
#	error "!!!undefined __SIZEOF_POINTER__!!!"
#endif

/* x86 32bit/64bit */
#undef LIBF_MARCH_TYPE_X86_32
#define LIBF_MARCH_TYPE_X86_32 1
#undef LIBF_MARCH_TYPE_X86_64
#define LIBF_MARCH_TYPE_X86_64 2
/* arm 32bit/64bit */
#undef LIBF_MARCH_TYPE_ARM_32
#define LIBF_MARCH_TYPE_ARM_32 3
#undef LIBF_MARCH_TYPE_ARM_64
#define LIBF_MARCH_TYPE_ARM_64 4
/* riscv 32bit/64bit */
#undef LIBF_MARCH_TYPE_RISCV_32
#define LIBF_MARCH_TYPE_RISCV_32 5
#undef LIBF_MARCH_TYPE_RISCV_64
#define LIBF_MARCH_TYPE_RISCV_64 6
/* mips 32bit/64bit */
#undef LIBF_MARCH_TYPE_MIPS_32
#define LIBF_MARCH_TYPE_MIPS_32 7
#undef LIBF_MARCH_TYPE_MIPS_64
#define LIBF_MARCH_TYPE_MIPS_64 8
/* ia64 64bit */
#undef LIBF_MARCH_TYPE_IA64_64
#define LIBF_MARCH_TYPE_IA64_64 10
/* ppc 64bit */
#undef LIBF_MARCH_TYPE_PPC_64
#define LIBF_MARCH_TYPE_PPC_64 11
/* s390 64bit */
#undef LIBF_MARCH_TYPE_S390_64
#define LIBF_MARCH_TYPE_S390_64 12
/* loongarch 64bit */
#undef LIBF_MARCH_TYPE_LOONGARCH_64
#define LIBF_MARCH_TYPE_LOONGARCH_64 9

/* machine architecture type */
#undef LIBF_MARCH_TYPE
#if defined(__i386__)
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_X86_32
#elif defined(__x86_64__)
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_X86_64
#elif (defined(__arm__) || defined(__thumb__) || defined(__ARM_EABI__))
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_ARM_32
#elif defined(__aarch64__)
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_ARM_64
#elif (defined(__riscv) && defined(__riscv_xlen))
#	if (__riscv_xlen == 32)
#		define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_RISCV_32
#	else
#		define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_RISCV_64
#	endif
#elif defined(__mips)
#	if defined(__mips64)
#		define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_MIPS_64
#	else
#		define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_MIPS_32
#	endif
#elif defined(__ia64__)
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_IA64_64
#elif defined(__powerpc64__)
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_PPC_64
#elif defined(__s390x__)
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_S390_64
#elif defined(__loongarch64)
#	define LIBF_MARCH_TYPE LIBF_MARCH_TYPE_LOONGARCH_64
#else
#	error "!!!unknown MARCH TYPE!!!"
#endif

/* no system (Bare Metal platform) */
#undef LIBF_PLATFORM_TYPE_NOSYS
#define LIBF_PLATFORM_TYPE_NOSYS 0
/* linux */
#undef LIBF_PLATFORM_TYPE_LINUX
#define LIBF_PLATFORM_TYPE_LINUX 1
/* freebsd */
#undef LIBF_PLATFORM_TYPE_FREEBSD
#define LIBF_PLATFORM_TYPE_FREEBSD 2
/* openbsd */
#undef LIBF_PLATFORM_TYPE_OPENBSD
#define LIBF_PLATFORM_TYPE_OPENBSD 2

/* platform */
#undef LIBF_PLATFORM_TYPE
#define LIBF_PLATFORM_TYPE LIBF_PLATFORM_TYPE_LINUX


#endif
