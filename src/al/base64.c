/* base64.c - base64 encode/decode implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/base64.h>


/* @def: base64 */
static const uint8 b64e[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789"
	"+/";

static const uint8 b64d[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	  7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	 19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	 37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	 49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
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

/* @func: base64_enc - base64 encode
* @param1: const char * # input buffer
* @param2: uint32       # input buffer length
* @param3: char *       # output buffer
* @param4: uint32 *     # output buffer length
* @return: void
*/
void FSYMBOL(base64_enc)(const char *s, uint32 slen, char *t, uint32 *tlen) {
	uint32 n = *tlen;
	while (slen) {
		*t++ = b64e[((uint8)s[0] >> 2) & 0x3f];
		if (!--n)
			break;

		*t++ = b64e[(((uint8)s[0] << 4)
			+ (--slen ? ((uint8)s[1] >> 4) : 0)) & 0x3f];
		if (!--n)
			break;

		*t++ = slen ? (b64e[(((uint8)s[1] << 2)
			+ (--slen ? ((uint8)s[2] >> 6) : 0)) & 0x3f]) : '=';
		if (!--n)
			break;

		*t++ = slen ? b64e[((uint8)s[2]) & 0x3f] : '=';
		if (!--n)
			break;

		if (slen)
			slen--;
		if (slen)
			s += 3;
	}
	*tlen = n;
} /* end */

/* @func: _base64_dec_4 (static) - base64 decode
* @param1: const char * # input buffer (8byte)
* @param2: char *       # output buffer
* @param3: uint32       # output buffer length
* @return: int32        # >0: output size, -1: output buffer full, \
*                         -2: base32 null character error
*/
static int32 _base64_dec_4(const char *s, char *t, uint32 tlen) {
	uint8 c0, c1, c2, c3;
	c0 = b64d[(uint8)s[0]];
	c1 = b64d[(uint8)s[1]];
	c2 = b64d[(uint8)s[2]];
	c3 = b64d[(uint8)s[3]];

	/* c-1 */
	if (!tlen--)
		return -1;
	*t++ = (c0 << 2) | (c1 >> 4);

	if (c2 == 254) { /* '=' */
		if (c3 == 254)
			return 1;
		return -2;
	}

	/* c-2 */
	if (!tlen--)
		return -1;
	*t++ = ((c1 << 4) & 0xf0) | (c2 >> 2);

	if (c3 == 254) /* '=' */
		return 2;

	/* c-3 */
	if (!tlen--)
		return -1;
	*t++ = ((c2 << 6) & 0xc0) | c3;

	return 3;
} /* end */

/* @func: base64_dec - base64 decode
* @param1: const char * # input buffer
* @param2: uint32       # input buffer length
* @param3: char *       # output buffer
* @param4: uint32 *     # output buffer length
* @return: int32        # 0: no error, >0: input error location, \
*                         -1: output buffer full, \
*                         -2: base32 null character error
*/
int32 FSYMBOL(base64_dec)(const char *s, uint32 slen, char *t, uint32 *tlen) {
	char buf[4];
	int32 k = 0;
	for (uint32 i = 0; i < slen; i++) {
		char c = s[i];
		if (b64d[(uint8)c] == 255)
			return i;

		buf[k++] = c;
		if (k == 4) {
			k = _base64_dec_4(buf, t, *tlen);
			if (k < 0)
				return k;
			t += k;
			*tlen -= k;
			k = 0;
		}
	}

	return 0;
} /* end */
