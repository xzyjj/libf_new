/* xmath_fpclassify.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: fpclassifyf - classify real floating-point type
* @param1: float32 # input value
* @return: int32   # floating-point type
*/
int32 XSYMBOL(fpclassifyf)(float32 x) {
	union float32_i u = { x };
	int32 e = (u.i >> 23) & 0xff;
	int32 f = u.i & 0x7fffff;

	if (!e)
		return f ? XFP_SUBNORMAL : XFP_ZERO;
	if (e == 0xff)
		return f ? XFP_NAN : XFP_INFINITE;

	return XFP_NORMAL;
} /* end */

/* @func: fpclassify - classify real floating-point type
* @param1: float64 # input value
* @return: int32   # floating-point type
*/
int32 XSYMBOL(fpclassify)(float64 x) {
	union float64_i u = { x };
	uint64L e = (u.i >> 52) & 0x7ff;
	uint64L f = u.i & 0xfffffffffffff;

	if (!e)
		return f ? XFP_SUBNORMAL : XFP_ZERO;
	if (e == 0x7ff)
		return f ? XFP_NAN : XFP_INFINITE;

	return XFP_NORMAL;
} /* end */
