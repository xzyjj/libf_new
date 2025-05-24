/* xmath_exp.h - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _exp (static) - exponential function (taylor expansion)
* @param1: float64 # input value
* @return: float64 # cardinality exponent of value
*/
static float64 _exp(float64 x) {
	float64 m = 1.0, q = 1.0;
	for (int32 i = 1; i <= 25; i++) {
		q *= x / i;
		m += q;
	}

	return m;
} /* end */

/* @func: exp - exponential function
* @param1: float64 # input value
* @return: float64 # cardinality exponent of value
*/
float64 XSYMBOL(exp)(float64 x) {
	return _exp(x);
} /* end */
