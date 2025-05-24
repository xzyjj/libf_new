/* xmath.h - mathematical definitions */

#ifndef LIBF_SL_XMATH_H
#define LIBF_SL_XMATH_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>


/* @def: math
* logarithm e */
#undef XM_E
#define XM_E 2.7182818284590452354
/* logarithm e of base 2 */
#undef XM_LOG2E
#define XM_LOG2E 2.7182818284590452354
/* logarithm e of base 10 */
#undef XM_LOG10E
#define XM_LOG10E 0.43429448190325182765
/* log(2) */
#undef XM_LN2
#define XM_LN2 0.69314718055994530942
/* log(10) */
#undef XM_LN10
#define XM_LN10 2.30258509299404568402
/* pi */
#undef XM_PI
#define XM_PI 3.14159265358979323846
/* pi/2 */
#undef XM_PI_2
#define XM_PI_2 1.57079632679489661923
/* pi/4 */
#undef XM_PI_4
#define XM_PI_4 0.78539816339744830962
/* 1/pi */
#undef XM_1_PI
#define XM_1_PI 0.31830988618379067154
/* 2/pi */
#undef XM_2_PI
#define XM_2_PI 0.63661977236758134308
/* 2/sqrt(pi) */
#undef XM_2_SQRTPI
#define XM_2_SQRTPI 1.12837916709551257390
/* square root of 2 */
#undef XM_SQRT2
#define XM_SQRT2 1.41421356237309504880
/* 1/sqrt(2) */
#undef XM_SQRT1_2
#define XM_SQRT1_2 0.70710678118654752440

#undef XFP_NAN
#define XFP_NAN 0
#undef XFP_INFINITE
#define XFP_INFINITE 1
#undef XFP_ZERO
#define XFP_ZERO 2
#undef XFP_SUBNORMAL
#define XFP_SUBNORMAL 3
#undef XFP_NORMAL
#define XFP_NORMAL 4
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xmath_acos.c */
extern float64 XSYMBOL(acos)(float64 x);

/* xmath_atan.c */
extern float64 XSYMBOL(atan)(float64 x);

/* xmath_cos.c */
extern float64 XSYMBOL(cos)(float64 x);

/* xmath_exp.c */
extern float64 XSYMBOL(exp)(float64 x);

/* xmath_fabs.c */
extern float64 XSYMBOL(fabs)(float64 x);

/* xmath_floor.c */
extern float64 XSYMBOL(floor)(float64 x);

/* xmath_fmod.c */
extern float64 XSYMBOL(fmod)(float64 x, float64 y);

/* xmath_fpclassify.c */
extern int32 XSYMBOL(fpclassifyf)(float32 x);
extern int32 XSYMBOL(fpclassify)(float64 x);

/* xmath_frexp.c */
extern float64 XSYMBOL(frexp)(float64 x, int32 *e);

/* xmath_log.c */
extern float64 XSYMBOL(log)(float64 x);
extern float64 XSYMBOL(log2)(float64 x);
extern float64 XSYMBOL(log10)(float64 x);

/* xmath_modf.c */
extern float64 XSYMBOL(modf)(float64 x, float64 *n);

/* xmath_pow.c */
extern float64 XSYMBOL(pow)(float64 x, float64 e);

/* xmath_sin.c */
extern float64 XSYMBOL(sin)(float64 x);

/* xmath_sqrt.c */
extern float64 XSYMBOL(sqrt)(float64 x);

/* xmath_tan.c */
extern float64 XSYMBOL(tan)(float64 x);

#ifdef __cplusplus
}
#endif


#endif
