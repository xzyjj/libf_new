/* bits_get.c - get bits from buffer implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bits_get.h>


/* @func: bits_get - get bits from the buffer
* @param1: struct bits_get_ctx * # bits-get struct context
* @param2: uint32                # bits value pointer
* @param3: uint32                # bits length
* @param4: int32                 # peek bits
* @return: int32                 # 0: not eof, -1: buffer eof, \
*                                  >1: remaining bits
*/
int32 FSYMBOL(bits_get)(struct bits_get_ctx *ctx, uint32 *v, uint32 len,
		int32 peek) {
	uint32 l = ctx->len;
	uint32 n = ctx->count;
	uint32 b = ctx->blen;

	if (n == l)
		return -1;

	static uint8 mask[9] = {
		0x00,
		0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
		};

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	uint32 _v = *v, c = 0, w = 0;
	_v = 0;

	while (n != l && len) {
		c = MIN(8 - b, len);
		_v |= ((ctx->buf[n] >> b) & mask[c]) << w;
		b += c;
		if (b == 8) {
			b = 0;
			n++;
		}
		w += c;
		len -= c;
	}

	*v = _v;

	if (!peek) {
		ctx->count = n;
		ctx->blen = b;
	}

	return (n == l) ? (len ? len : -1) : 0;
} /* end */
