/* xmath_sin.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _sin (static) - sine function (taylor series running product)
* @param1: float64 # input value
* @return: float64 # sine of value
*/
static float64 _sin(float64 x) {
	int64L dneg = (int64L)(x / XM_PI);
	x = x - (dneg * XM_PI);
	dneg = (dneg % 2) ? -1 : 1;

#if 0

	int32 sign = -1;
	float64 m = x, q = x, x2 = x * x;
	for (int32 i = 1; i <= 10; i++) {
		q *= x2 / ((2 * i) * (2 * i + 1));
		m += sign * q;
		sign *= -1;
	}

	return m * dneg;

#else

	/* https://austinhenley.com/blog/cosine.html */
	float64 m = x, q = x, x2 = x * x;
	for (int32 i = 1; i <= 10; i++) {
		float64 comp = 2.0 * i + 1.0;
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

/* @func: sin - sine function
* @param1: float64 # input value
* @return: float64 # sine of value
*/
float64 XSYMBOL(sin)(float64 x) {
	return _sin(x);
} /* end */
