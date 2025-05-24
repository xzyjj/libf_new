/* xstrto_ultostr.c - string and number convert implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstrto.h>


/* @func: ulltostr_o - number convert string (octal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(ulltostr_o)(int32 n, char *p, uint64L v) {
	if (v >= 8)
		n = XSYMBOL(ulltostr_o)(n, p, v / 8);
	p[n] = (v % 8) + '0';

	return ++n;
} /* end */

/* @func: ulltostr_d - number convert string (decimal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(ulltostr_d)(int32 n, char *p, uint64L v) {
	if (v >= 10)
		n = XSYMBOL(ulltostr_d)(n, p, v / 10);
	p[n] = (v % 10) + '0';

	return ++n;
} /* end */

/* @func: ulltostr_x - number convert string (hexadecimal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(ulltostr_x)(int32 n, char *p, uint64L v) {
	if (v >= 16)
		n = XSYMBOL(ulltostr_x)(n, p, v / 16);
	v %= 16;
	p[n] = (v <= 9) ? (v + '0') : (v + 'a' - 10);

	return ++n;
} /* end */

/* @func: ulltostr_X - number convert string (uppercase hexadecimal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: uint64L # unsigned long long
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(ulltostr_X)(int32 n, char *p, uint64L v) {
	if (v >= 16)
		n = XSYMBOL(ulltostr_X)(n, p, v / 16);
	v %= 16;
	p[n] = (v <= 9) ? (v + '0') : (v + 'A' - 10);

	return ++n;
} /* end */

/* @func: ultostr - number convert string
* @param1: char *  # string buffer
* @param2: uint64  # unsigned long int
* @param3: int32   # base type
* @return: char *  # buffer pointer
*/
char *XSYMBOL(ultostr)(char *buf, uint64 v, int32 type) {
	int32 n = 0;
	switch (type) {
		case 8:
			n = XSYMBOL(ulltostr_o)(0, buf, v);
			break;
		case 10:
			n = XSYMBOL(ulltostr_d)(0, buf, v);
			break;
		case 16:
			n = XSYMBOL(ulltostr_x)(0, buf, v);
			break;
		case 17:
			n = XSYMBOL(ulltostr_X)(0, buf, v);
			break;
		default:
			/* errno */
			break;
	}
	buf[n] = '\0';

	return buf;
} /* end */

/* @func: ulltostr - number convert string
* @param1: char *  # string buffer
* @param2: uint64L # unsigned long long
* @param3: int32   # base type
* @return: char *  # buffer pointer
*/
char *XSYMBOL(ulltostr)(char *buf, uint64L v, int32 type) {
	int32 n = 0;
	switch (type) {
		case 8:
			n = XSYMBOL(ulltostr_o)(0, buf, v);
			break;
		case 10:
			n = XSYMBOL(ulltostr_d)(0, buf, v);
			break;
		case 16:
			n = XSYMBOL(ulltostr_x)(0, buf, v);
			break;
		case 17:
			n = XSYMBOL(ulltostr_X)(0, buf, v);
			break;
		default:
			/* errno */
			break;
	}
	buf[n] = '\0';

	return buf;
} /* end */
