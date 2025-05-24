/* xmath_atan.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


#if 0

/* @func: _atan (static) - arc tangent function (taylor series running product)
* @param1: float64 # input value
* @return: float64 # arc tangent of value
*/
static float64 _atan(float64 x) {
	if (x > 1.0) {
		return XM_PI_2 - _atan(1.0 / x);
	} else if (x < -1.0) {
		return -XM_PI_2 - _atan(1.0 / x);
	}

	float64 m = x, q = x, x2 = x * x;
	for (int32 i = 1; i <= 10; i++) {
		q *= -x2;
		m += q / (2 * i + 1);
	}

	return m;
} /* end */

#else

/* @func: _atan (static) - arc tangent function (precomputed coefficients)
* @param1: float64 # input value
* @return: float64 # arc tangent of value
*/
static float64 _atan(float64 x) {
	if (x > 1.0) {
		return XM_PI_2 - _atan(1.0 / x);
	} else if (x < -1.0) {
		return -XM_PI_2 - _atan(1.0 / x);
	}

	static const float64 coeffs[10] = {
		1.0,
		-1.0 / 3,
		1.0 / 5,
		-1.0 / 7,
		1.0 / 9,
		-1.0 / 11,
		1.0 / 13,
		-1.0 / 15,
		1.0 / 17,
		-1.0 / 19
		};

	float64 m = 0.0, p = x;
	for (int32 i = 0; i < 10; i++) {
		m += coeffs[i] * p;
		p *= x * x;
	}

	return m;
} /* end */

#endif

/* @func: atan - arc tangent function
* @param1: float64 # input value
* @return: float64 # arc tangent of value
*/
float64 XSYMBOL(atan)(float64 x) {
	return _atan(x);
} /* end */
