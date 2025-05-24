/* xmath_pow.h - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _pow (static) - power function
* @param1: float64 # input value
* @param2: float64 # exponent value
* @return: float64 # return x^e power value
*/
static float64 _pow(float64 x, float64 e) {
	float64 m = 1.0;
	uint64L abs_e = (uint64L)XSYMBOL(fabs)(e);
	while (abs_e > 0) {
		if (abs_e % 2 == 1)
			m *= x;
		abs_e >>= 1;
		x *= x;
	}
	m = (e < 0.0) ? (1.0 / m) : m;

	return m;
} /* end */

/* @func: pow - power function
* @param1: float64 # input value
* @param2: float64 # exponent value
* @return: float64 # return x^e power value
*/
float64 XSYMBOL(pow)(float64 x, float64 e) {
	float64 m = 0.0, q = 0.0;
	m = _pow(XSYMBOL(fabs)(x), e);

	q = e - (int64L)e;
	if (q != 0.0)
		m *= XSYMBOL(exp)(q * XSYMBOL(log)(x));

	return m;
} /* end */
