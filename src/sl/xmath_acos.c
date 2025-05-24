/* xmath_acos.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _acos (static) - arc cosine function ()
* @param1: float64 # input value
* @return: float64 # arc cosine of value
*/
static float64 _acos(float64 x) {
	float64 m = XM_PI_2;
	for (int32 i = 0; i < 5; i++)
		m -= (XSYMBOL(cos)(m) - x) / (-XSYMBOL(sin)(m));

	return m;
} /* end */

/* @func: acos - arc cosine function
* @param1: float64 # input value
* @return: float64 # arc cosine of value
*/
float64 XSYMBOL(acos)(float64 x) {
	if (x < -1.0 || x > 1.0)
		return NAN;

	return _acos(x);
} /* end */
