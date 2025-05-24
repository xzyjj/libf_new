/* xstdint.h - integer types definitions */

#ifndef LIBF_SL_XSTDINT_H
#define LIBF_SL_XSTDINT_H

#include <libf/config.h>


/* @def: stdint */
#undef int8
typedef signed char int8;
#undef uint8
typedef unsigned char uint8;
#undef int16
typedef signed short int16;
#undef uint16
typedef unsigned short uint16;
#undef int32
typedef signed int int32;
#undef uint32
typedef unsigned int uint32;
#undef int64
typedef signed long int int64;
#undef uint64
typedef unsigned long int uint64;
#undef int64L
typedef signed long long int64L;
#undef uint64L
typedef unsigned long long uint64L;

#undef INT8_MIN
#define INT8_MIN (-0x80)
#undef INT8_MAX
#define INT8_MAX 0x7f
#undef UINT8_MAX
#define UINT8_MAX 0xff

#undef INT16_MIN
#define INT16_MIN (-0x8000)
#undef INT16_MAX
#define INT16_MAX 0x7fff
#undef UINT16_MAX
#define UINT16_MAX 0xffff

#undef INT32_MIN
#define INT32_MIN (-0x80000000)
#undef INT32_MAX
#define INT32_MAX 0x7fffffff
#undef UINT32_MAX
#define UINT32_MAX 0xffffffff

#undef INT64_MIN
#undef INT64_MAX
#undef UINT64_MAX
#ifdef LIBF_MARCH_BIT
#	if (LIBF_MARCH_BIT == LIBF_MARCH_BIT_32)
#		define INT64_MIN (-0x80000000)
#		define INT64_MAX 0x7fffffff
#		define UINT64_MAX 0xffffffff
#	elif (LIBF_MARCH_BIT == LIBF_MARCH_BIT_64)
#		define INT64_MIN (-0x8000000000000000L)
#		define INT64_MAX 0x7fffffffffffffffL
#		define UINT64_MAX 0xffffffffffffffffUL
#	else
#		error "!!!unknown LIBF_MARCH_BIT!!!"
#	endif
#else
#	error "!!!undefined LIBF_MARCH_BIT!!!"
#endif

#undef INT64L_MIN
#define INT64L_MIN (-0x8000000000000000ULL)
#undef INT64L_MAX
#define INT64L_MAX 0x7fffffffffffffffULL
#undef UINT64L_MAX
#define UINT64L_MAX 0xffffffffffffffffULL
/* end */


#endif
