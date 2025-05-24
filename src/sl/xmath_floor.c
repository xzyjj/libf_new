/* xmath_floor.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: floor - compute the maximum integer value of x
* @param1: float64 # input value
* @return: float64 # maximum integer value
*/
float64 XSYMBOL(floor)(float64 x) {
	union float64_i u = { x };
	uint32 e = ((u.i >> 52) & 0x7ff) - 1023;

	if (e > 51)
		return x;
	if (e < 0)
		return (u.i << 63) ? -1.0 : 0.0;

	uint64L m = 1ULL << (52 - e);
	if (!(u.i & (m - 1)))
		return x;

	if (x < 0.0)
		u.i += m;
	u.i &= ~(m - 1);

	return u.f;
} /* end */
