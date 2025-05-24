/* xmath_sqrt.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


#if 0

/* @func: _sqrt (static) - square root function (binary search)
* @param1: float64 # input value
* @return: float64 # square root of value
*/
static float64 _sqrt(float64 x) {
	float64 a = 0, b = x + 1;
	for (int32 i = 0; i < 60; i++) {
		float64 m = (a + b) / 2;
		if ((m * m) < x) {
			a = m;
		} else {
			b = m;
		}
	}

	return a;
} /* end */

#else
#	if 0

/* @func: _sqrt (static) - square root function (newton-raphson method)
* @param1: float64 # input value
* @return: float64 # square root of value
*/
static float64 _sqrt(float64 x) {
	float64 m = x, q = 1.0;
	for (int32 i = 0; i < 20; i++) {
		m = (m + q) / 2;
		q = x / m;
	}

	return m;
} /* end */

#	else

/* @func: _sqrt (static) - square root function (newton-raphson method)
* @param1: float64 # input value
* @return: float64 # square root of value
*/
static float64 _sqrt(float64 x) {
	union float64_i u = { x };
	u.i = 0x1ff7a3bea91d9b1b + (u.i >> 1);
	float64 m = u.f;
	m = 0.5 * (m + x / m);
	m = 0.5 * (m + x / m);
	m = 0.5 * (m + x / m);
	m = 0.5 * (m + x / m);

	return m;
} /* end */

#	endif
#endif

/* @func: sqrt - square root function
* @param1: float64 # input value
* @return: float64 # square root of value
*/
float64 XSYMBOL(sqrt)(float64 x) {
	if (x < 0.0)
		return NAN;

	return _sqrt(x);
} /* end */
