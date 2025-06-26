/* xiconv_utf8.c - convert unicode encode implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xiconv.h>


/* @func: iconv_utf8_ucode - get utf8 unicode code
* @param1: const uint8 [4] # utf8 character
* @return: int32           # unicode code
*/
int32 XSYMBOL(iconv_utf8_ucode)(const uint8 c[4]) {
	int32 n = -1;
	if (c[0] < 128) {
		n = c[0];
	} else if ((c[0] & 0xe0) == 0xc0) {
		n = (c[0] & 0x1f) << 6;
		n |= c[1] & 0x3f;
	} else if ((c[0] & 0xf0) == 0xe0) {
		n = (c[0] & 0x0f) << 12;
		n |= (c[1] & 0x3f) << 6;
		n |= c[2] & 0x3f;
	} else if ((c[0] & 0xf8) == 0xf0) {
		n = (c[0] & 0x07) << 18;
		n |= (c[1] & 0x3f) << 12;
		n |= (c[2] & 0x3f) << 6;
		n |= c[3] & 0x3f;
	}

	return n;
} /* end */

/* @func: iconv_ucode_utf8 - unicode code to utf8 character
* @param1: uint8 [4] # character buffer
* @param2: int32     # unicode code
* @return: int32     # character length
*/
int32 XSYMBOL(iconv_ucode_utf8)(uint8 p[4], int32 u) {
	int32 n = 0;
	if (u < 128) {
		p[0] = u;
		n = 1;
	} else if (u <= 0x7ff) {
		p[0] = 0xc0 | (u >> 6);
		p[1] = 0x80 | (u & 0x3f);
		n = 2;
	} else if (u <= 0xffff) {
		p[0] = 0xe0 | (u >> 12);
		p[1] = 0x80 | ((u >> 6) & 0x3f);
		p[2] = 0x80 | (u & 0x3f);
		n = 3;
	} else if (u <= 0x10ffff) {
		p[0] = 0xe0 | (u >> 18);
		p[1] = 0x80 | (u >> 12);
		p[2] = 0x80 | ((u >> 6) & 0x3f);
		p[3] = 0x80 | (u & 0x3f);
		n = 4;
	}

	return n;
} /* end */
