/* base32.c - base32 encode/decode implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/base32.h>


/* @def: base32 */
static const uint8 b32e[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"234567";

static const uint8 b32d[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255,  26,  27,  28,  29,  30,  31, 255, 255, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	  7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	 19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
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
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
	};
/* end */

/* @func: base32_enc - base32 encode
* @param1: const char * # input buffer
* @param2: uint32       # input buffer length
* @param3: char *       # output buffer
* @param4: uint32 *     # output buffer length
* @return: void
*/
void FSYMBOL(base32_enc)(const char *s, uint32 slen, char *t, uint32 *tlen) {
	uint32 n = *tlen;
	while (slen) {
		*t++ = b32e[((uint8)s[0] >> 3) & 0x1f];
		if (!--n)
			break;

		*t++ = b32e[(((uint8)s[0] << 2)
			+ (--slen ? ((uint8)s[1] >> 6) : 0)) & 0x1f];
		if (!--n)
			break;

		*t++ = slen ? b32e[((uint8)s[1] >> 1) & 0x1f] : '=';
		if (!--n)
			break;

		*t++ = slen ? b32e[(((uint8)s[1] << 4)
			+ (--slen ? ((uint8)s[2] >> 4) : 0)) & 0x1f] : '=';
		if (!--n)
			break;

		*t++ = slen ? b32e[(((uint8)s[2] << 1)
			+ (--slen ? ((uint8)s[3] >> 7) : 0)) & 0x1f] : '=';
		if (!--n)
			break;

		*t++ = slen ? b32e[((uint8)s[3] >> 2) & 0x1f] : '=';
		if (!--n)
			break;

		*t++ = slen ? b32e[(((uint8)s[3] << 3)
			+ (--slen ? ((uint8)s[4] >> 5) : 0)) & 0x1f] : '=';
		if (!--n)
			break;

		*t++ = slen ? b32e[(uint8)s[4] & 0x1f] : '=';
		if (!--n)
			break;

		if (slen)
			slen--;
		if (slen)
			s += 5;
	}
	*tlen = n;
} /* end */

/* @func: _base32_dec_8 (static) - base32 decode
* @param1: const char * # input buffer (8byte)
* @param2: char *       # output buffer
* @param3: uint32       # output buffer length
* @return: int32        # >0: output size, -1: output buffer full, \
*                         -2: base32 null character error
*/
static int32 _base32_dec_8(const char *s, char *t, uint32 tlen) {
	uint8 c0, c1, c2, c3, c4, c5, c6, c7;
	c0 = b32d[(uint8)s[0]];
	c1 = b32d[(uint8)s[1]];
	c2 = b32d[(uint8)s[2]];
	c3 = b32d[(uint8)s[3]];
	c4 = b32d[(uint8)s[4]];
	c5 = b32d[(uint8)s[5]];
	c6 = b32d[(uint8)s[6]];
	c7 = b32d[(uint8)s[7]];

	/* c-1 */
	if (!tlen--)
		return -1;
	*t++ = (c0 << 3) | (c1 >> 2);

	if (c2 == 254) { /* '=' */
		if (c3 == 254 && c4 == 254 && c5 == 254
				&& c6 == 254 && c7 == 254)
			return 1;
		return -2;
	}

	/* c-2 */
	if (!tlen--)
		return -1;
	*t++ = (c1 << 6) | (c2 << 1 ) | (c3 >> 4);

	if (c4 == 254) { /* '=' */
		if (c5 == 254 && c6 == 254 && c7 == 254)
			return 2;
		return -2;
	}

	/* c-3 */
	if (!tlen--)
		return -1;
	*t++ = (c3 << 4) | (c4 >> 1);

	if (c5 == 254) { /* '=' */
		if (c6 == 254 && c7 == 254)
			return 3;
		return -2;
	}

	/* c-4 */
	if (!tlen--)
		return -1;
	*t++ = (c4 << 7) | (c5 << 2) | (c6 >> 3);

	if (c7 == 254) /* '=' */
		return 4;

	/* c-5 */
	if (!tlen--)
		return -1;
	*t++ = (c6 << 5) | c7;

	return 5;
} /* end */

/* @func: base32_dec - base32 decode
* @param1: const char * # input buffer
* @param2: uint32       # input buffer length
* @param3: char *       # output buffer
* @param4: uint32 *     # output buffer length
* @return: int32        # 0: no error, >0: input error location, \
*                         -1: output buffer full, \
*                         -2: base32 null character error
*/
int32 FSYMBOL(base32_dec)(const char *s, uint32 slen, char *t, uint32 *tlen) {
	char buf[8];
	int32 k = 0;
	for (uint32 i = 0; i < slen; i++) {
		char c = s[i];
		if (b32d[(uint8)c] == 255)
			return i;

		buf[k++] = c;
		if (k == 8) {
			k = _base32_dec_8(buf, t, *tlen);
			if (k < 0)
				return k;
			t += k;
			*tlen -= k;
			k = 0;
		}
	}

	return 0;
} /* end */
