/* internal_itoa.c - number to string conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/internal.h>


/* @func: internal_ulltostr_o - number convert string (octal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(internal_ulltostr_o)(int32 n, char *p, uint64L v) {
	if (v >= 8)
		n = XSYMBOL(internal_ulltostr_o)(n, p, v / 8);
	p[n] = (v % 8) + '0';

	return ++n;
} /* end */

/* @func: internal_ulltostr_d - number convert string (decimal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(internal_ulltostr_d)(int32 n, char *p, uint64L v) {
	if (v >= 10)
		n = XSYMBOL(internal_ulltostr_d)(n, p, v / 10);
	p[n] = (v % 10) + '0';

	return ++n;
} /* end */

/* @func: internal_ulltostr_x - number convert string (hexadecimal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(internal_ulltostr_x)(int32 n, char *p, uint64L v) {
	if (v >= 16)
		n = XSYMBOL(internal_ulltostr_x)(n, p, v / 16);
	v %= 16;
	p[n] = (v <= 9) ? (v + '0') : (v + 'a' - 10);

	return ++n;
} /* end */

/* @func: internal_ulltostr_X - number convert string (uppercase hexadecimal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(internal_ulltostr_X)(int32 n, char *p, uint64L v) {
	if (v >= 16)
		n = XSYMBOL(internal_ulltostr_X)(n, p, v / 16);
	v %= 16;
	p[n] = (v <= 9) ? (v + '0') : (v + 'A' - 10);

	return ++n;
} /* end */
