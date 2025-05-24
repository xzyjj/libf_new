/* xfloat.h - floating-point types definitions */

#ifndef LIBF_SL_XFLOAT_H
#define LIBF_SL_XFLOAT_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: float */
#undef float32
typedef float float32;
#undef float64
typedef double float64;

/* base */
#undef XFP_FLT_RADIX
#define XFP_FLT_RADIX 2
/* mantissa */
#undef XFP_FLT_MANT_DIG
#define XFP_FLT_MANT_DIG 24
/* exact number of significant digits */
#undef XFP_FLT_DIG
#define XFP_FLT_DIG 6
/* accuracy error range */
#undef XFP_FLT_EPSILON
#define XFP_FLT_EPSILON 1.19209290e-7
/* exponent */
#undef XFP_FLT_MIN_EXP
#define XFP_FLT_MIN_EXP (-125)
#undef XFP_FLT_MAX_EXP
#define XFP_FLT_MAX_EXP 128
/* size */
#undef XFP_FLT_MIN
#define XFP_FLT_MIN 1.17549435e-38
#undef XFP_FLT_MAX
#define XFP_FLT_MAX 3.40282347e+38

/* base */
#undef XFP_DBL_RADIX
#define XFP_DBL_RADIX 2
/* mantissa */
#undef XFP_DBL_MANT_DIG
#define XFP_DBL_MANT_DIG 53
/* exact number of significant digits */
#undef XFP_DBL_DIG
#define XFP_DBL_DIG 15
/* accuracy error range */
#undef XFP_DBL_EPSILON
#define XFP_DBL_EPSILON 2.2204460492503131e-16
/* exponent */
#undef XFP_DBL_MIN_EXP
#define XFP_DBL_MIN_EXP (-1021)
#undef XFP_DBL_MAX_EXP
#define XFP_DBL_MAX_EXP 1024
/* size */
#undef XFP_DBL_MIN
#define XFP_DBL_MIN 2.2250738585072014e-308
#undef XFP_DBL_MAX
#define XFP_DBL_MAX 1.7976931348623157e+308

/* float32 convert uint32 integer */
union float32_i {
	float32 f;
	uint32 i;
};

/* float64 convert uint64L integer */
union float64_i {
	float64 f;
	uint64L i;
};

static union float32_i _ZERO = { .i = 0x0 };
static union float32_i _NAN = { .i = 0x7fc00000 };
static union float32_i _INFINITY = { .i = 0x7f800000 };

#undef ZERO
#define ZERO (_ZERO.f)
#undef NAN
#define NAN (_NAN.f)
#undef INFINITY
#define INFINITY (_INFINITY.f)
/* end */


#endif
