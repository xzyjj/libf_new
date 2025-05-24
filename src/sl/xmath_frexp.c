/* xmath_frexp.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: frexp - extract the mantissa and exponent from floating-point
* @param1: float64 # input value
* @param2: int32 * # exponent
* @return: float64 # mantissa
*/
float64 XSYMBOL(frexp)(float64 x, int32 *e) {
	union float64_i u = { x };
	int32 ee = (u.i >> 52) & 0x7ff;

	if (!ee) {
		if (x) {
			x = XSYMBOL(frexp)(x * 0x1p64, e);
			*e -= 64;
		} else {
			*e = 0;
		}
		return x;
	} else if (ee == 0x7ff) {
		return x;
	}

	*e = ee - 1022;
	u.i &= 0x800fffffffffffffULL;
	u.i |= 0x3fe0000000000000ULL;

	return u.f;
} /* end */
