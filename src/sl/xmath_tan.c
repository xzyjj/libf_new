/* xmath_tan.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _tan (static) - tangent function
* @param1: float64 # input value
* @return: float64 # tangent of value
*/
static float64 _tan(float64 x) {
	if (x > XM_PI)
		x = XSYMBOL(fmod)(x, XM_PI);

	float64 m = XSYMBOL(sin)(x);
	float64 v = XSYMBOL(cos)(x);
	if (v == 0.0)
		return 0.0;

	return m / v;
} /* end */

/* @func: tan - tangent function
* @param1: float64 # input value
* @return: float64 # tangent of value
*/
float64 XSYMBOL(tan)(float64 x) {
	return _tan(x);
} /* end */
