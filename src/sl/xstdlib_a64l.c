/* xstdlib_a64l.c - number to base64 conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdlib.h>


/* @func: a64l - base-64 convert long
* @param1: char * # string pointer
* @return: uint64 # 6byte base64 to long
*/
uint64 XSYMBOL(a64l)(const char *s) {
	uint64 n = 0;
	for (int32 i = 0; *s != '\0' && i < 36; i += 6) {
		int32 d = 0;
		if (*s == '.') {
			d = 0;
		} else if (*s == '/') {
			d = 1;
		} else if (*s >= '0' && *s <= '9') {
			d = *s - '0' + 2;
		} else if (*s >= 'A' && *s <= 'Z') {
			d = *s - 'A' + 12;
		} else if (*s >= 'a' && *s <= 'z') {
			d = *s - 'a' + 38;
		} else {
			break;
		}

		n |= (d << i);
		s++;
	}

	return n;
} /* end */

/* @func: l64a_r - long convert base-64
* @param1: char * # buffer pointer (7byte)
* @param2: uint64 # long value
* @return: char * # long to 6byte base64
*/
char *XSYMBOL(l64a_r)(char *b, uint64 n) {
	char *p = b;
	for (int32 i = 0; n && i < 6; i++) {
		int32 d = n & 0x3f;
		if (d == 0) {
			*p = '.';
		} else if (d == 1) {
			*p = '/';
		} else if (d < 12) {
			*p = '0' + (d - 2);
		} else if (d < 38) {
			*p = 'A' + (d - 12);
		} else {
			*p = 'a' + (d - 38);
		}

		n >>= 6;
		p++;
	}
	*p = '\0';

	return b;
} /* end */
