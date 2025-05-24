/* xmath_fabs.c - mathematical implementations */

#include <libf/config.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xmath.h>


/* @func: fabs - return an floating-point absolute value
* @param1: float64 # input value
* @return: float64 # floating-point absolute value
*/
float64 XSYMBOL(fabs)(float64 x) {
	return (x < 0.0) ? -x : x;
} /* end */
