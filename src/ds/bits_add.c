/* bits_add.c - add bits to buffer implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bits_add.h>


/* @func: bits_add - add bits to the buffer
* @param1: struct bits_add_ctx * # bits-add struct context
* @param2: uint32                # bits value
* @param3: uint32                # bits length
* @return: int32                 # 0: not full, 1: buffer full
*/
int32 FSYMBOL(bits_add)(struct bits_add_ctx *ctx, uint32 v, uint32 len) {
	uint32 n = ctx->count;
	uint32 b = ctx->blen;
	uint32 c = ctx->brem;

	if (n == BITS_ADD_BUFSIZE)
		return 1;
	if (c) {
		v >>= len - c;
		len = c;
	}

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	while (n != BITS_ADD_BUFSIZE && len) {
		c = 8 - b;
		ctx->buf[n] |= (v & ((1 << c) - 1)) << b;
		b += c = MIN(c, len);
		if (b == 8) {
			b = 0;
			n++;
		}
		v >>= c;
		len -= c;
	}

	ctx->count = n;
	ctx->blen = b;
	ctx->brem = len;

	return (n == BITS_ADD_BUFSIZE) ? 1 : 0;
} /* end */
