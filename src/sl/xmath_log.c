/* xmath_log.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _log (static) - natural logarithm function (newton-raphson method)
* @param1: float64 # input value
* @return: float64 # natural logarithm of value
*/
static float64 _log(float64 x) {
	if (x <= 0.0)
		return 0.0;

	float64 m = x, q = 0.0;
	for (int32 i = 0; i < 15; i++) {
		q = XSYMBOL(exp)(m);
		m -= (q - x) / q;
	}

	return m;
} /* end */

/* @func: log - natural logarithm function
* @param1: float64 # input value
* @return: float64 # natural logarithm of value
*/
float64 XSYMBOL(log)(float64 x) {
	return _log(x);
} /* end */

/* @func: log2 - natural logarithm function of base 2
* @param1: float64 # input value
* @return: float64 # natural logarithm of value
*/
float64 XSYMBOL(log2)(float64 x) {
	return _log(x) / XM_LN2;
} /* end */

/* @func: log10 - natural logarithm function of base 10
* @param1: float64 # input value
* @return: float64 # natural logarithm of value
*/
float64 XSYMBOL(log10)(float64 x) {
	return _log(x) / XM_LN10;
} /* end */
