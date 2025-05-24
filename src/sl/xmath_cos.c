/* xmath_cos.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _cos (static) - cosine function (taylor series running product)
* @param1: float64 # input value
* @return: float64 # cosine of value
*/
static float64 _cos(float64 x) {
	int64L dneg = (int64L)(x / XM_PI);
	x = x - (dneg * XM_PI);
	dneg = (dneg % 2) ? -1 : 1;

#if 1

	int32 sign = -1;
	float64 m = 1.0, q = 1.0, x2 = x * x;
	for (int32 i = 1; i <= 10; i++) {
		q *= x2 / ((2 * i - 1.0) * (2 * i));
		m += sign * q;
		sign *= -1;
	}

	return m * dneg;

#else

	/* https://austinhenley.com/blog/cosine.html */
	float64 m = 1.0, q = 1.0, x2 = x * x;
	for (int32 i = 1; i <= 10; i++) {
		float64 comp = 2.0 * i;
		float64 den = comp * (comp - 1.0);
		q *= x2 / den;
		if (i % 2) {
			m -= q;
		} else {
			m += q;
		}
	}

	return m * dneg;

#endif
} /* end */

/* @func: cos - cosine function
* @param1: float64 # input value
* @return: float64 # cosine of value
*/
float64 XSYMBOL(cos)(float64 x) {
	return _cos(x);
} /* end */
