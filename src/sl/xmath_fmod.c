/* xmath_fmod.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: _fmod (static) - remainder function
* @param1: float64 # input value
* @param2: float64 # denominator
* @return: float64 # remainder
*/
static float64 _fmod(float64 x, float64 y) {
	float64 n = XSYMBOL(floor)(x / y);

	return x - (n * y);
} /* end */

/* @func: fmod - remainder function
* @param1: float64 # input value
* @param2: float64 # denominator
* @return: float64 # remainder
*/
float64 XSYMBOL(fmod)(float64 x, float64 y) {
	return _fmod(x, y);
} /* end */
