/* base16.c - base16 encode/decode implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/base16.h>


/* @def: base16 */
static const uint8 b16e[] = "0123456789"
	"ABCDEF";

static const uint8 b16d[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255,
	255, 255, 255, 255, 255,  10,  11,  12,  13,  14,  15, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255,  10,  11,  12,  13,  14,  15, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
	};
/* end */

/* @func: base16_enc - base16 encode
* @param1: const char * # input buffer
* @param2: uint32       # input buffer length
* @param3: char *       # output buffer
* @param4: uint32 *     # remaining length of output buffer
* @return: void
*/
void FSYMBOL(base16_enc)(const char *s, uint32 slen, char *t, uint32 *tlen) {
	uint32 n = *tlen;
	while (slen) {
		*t++ = b16e[(uint8)s[0] >> 4];
		if (!--n)
			break;

		*t++ = b16e[(uint8)s[0] & 0x0f];
		if (!--n)
			break;

		slen--;
		s++;
	}
	*tlen = n;
} /* end */

/* @func: _base16_dec_2 (static) - base16 decode
* @param1: const char * # input buffer (2byte)
* @param2: char *       # output buffer
* @param3: uint32       # output buffer length
* @return: int32        # >0: output size, -1: output buffer full
*/
static int32 _base16_dec_2(const char *s, char *t, uint32 tlen) {
	uint8 c0, c1;
	c0 = b16d[(uint8)s[0]];
	c1 = b16d[(uint8)s[1]];

	/* c-1 */
	if (!tlen--)
		return -1;
	*t++ = (c0 << 4) | c1;

	return 1;
} /* end */

/* @func: base16_dec - base16 decode
* @param1: const char * # input buffer
* @param2: uint32       # input buffer length
* @param3: char *       # output buffer
* @param4: uint32 *     # remaining length of output buffer
* @return: int32        # 0: no error, >0: input error location, \
*                         -1: output buffer full
*/
int32 FSYMBOL(base16_dec)(const char *s, uint32 slen, char *t, uint32 *tlen) {
	char buf[2];
	int32 k = 0;
	for (uint32 i = 0; i < slen; i++) {
		char c = s[i];
		if (b16d[(uint8)c] == 255)
			return i;

		buf[k++] = c;
		if (k == 2) {
			k = _base16_dec_2(buf, t, *tlen);
			if (k < 0)
				return k;
			t += k;
			*tlen -= k;
			k = 0;
		}
	}

	return 0;
} /* end */
