/* xmath_modf.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: modf - extract floating-point integer and decimals
* @param1: float64   # input value
* @param2: float64 * # integer
* @return: float64   # decimals
*/
float64 XSYMBOL(modf)(float64 x, float64 *n) {
	union float64_i u = { x };
	uint32 e = ((u.i >> 52) & 0x7ff) - 1023;

	if (e > 51) {
		*n = x;
		return 0.0;
	}
	if (e < 0) {
		*n = 0.0;
		return x;
	}

	uint64L m = 1ULL << (52 - e);
	if (!(u.i & (m - 1))) {
		*n = x;
		u.i &= 0x8000000000000000ULL;
		return u.f;
	}

	u.i &= ~(m - 1);
	*n = u.f;

	return x - u.f;
} /* end */
