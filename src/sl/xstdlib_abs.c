/* xstdlib_abs.c - absolute value integer calculation implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdlib.h>


/* @func: abs - return an integer absolute value
* @param1: int32 # int value
* @return: int32 # integer absolute value
*/
int32 XSYMBOL(abs)(int32 n) {
	return (n < 0) ? -n : n;
} /* end */

/* @func: labs - return an integer absolute value
* @param1: int64 # long int value
* @return: int64 # integer absolute value
*/
int64 XSYMBOL(labs)(int64 n) {
	return (n < 0) ? -n : n;
} /* end */

/* @func: llabs - return an integer absolute value
* @param1: int64L # long long value
* @return: int64L # integer absolute value
*/
int64L XSYMBOL(llabs)(int64L n) {
	return (n < 0) ? -n : n;
} /* end */
