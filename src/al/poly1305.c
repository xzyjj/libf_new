/* poly1305.c - poly1305 message authentication code implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/poly1305.h>


/* @def: poly1305
* P = 2**130 - 5
* h = ((h + m) * r) % P */

#undef PACK4
#define PACK4(x) ((uint32)((x)[0]) | (uint32)((x)[1]) << 8 \
	| (uint32)((x)[2]) << 16 | (uint32)((x)[3]) << 24)
/* end */

/* @func: poly1305_init - poly1305 init function
* @param1: struct poly1305_ctx * # poly1305 struct context
* @param2: const uint8 * # key (length: POLY1305_KEYLEN)
* @return: void
*/
void FSYMBOL(poly1305_init)(struct poly1305_ctx *ctx, const uint8 *key) {
	ctx->r[0] = PACK4(key) & 0x3ffffff;
	ctx->r[1] = (PACK4(key + 3) >> 2) & 0x3ffff03;
	ctx->r[2] = (PACK4(key + 6) >> 4) & 0x3ffc0ff;
	ctx->r[3] = (PACK4(key + 9) >> 6) & 0x3f03fff;
	ctx->r[4] = (PACK4(key + 12) >> 8) & 0x00fffff;

	ctx->s[0] = PACK4(key + 16);
	ctx->s[1] = PACK4(key + 20);
	ctx->s[2] = PACK4(key + 24);
	ctx->s[3] = PACK4(key + 28);

	XSYMBOL(memset)(ctx->h, 0, sizeof(ctx->h));
	ctx->count = 0;
} /* end */

/* @func: poly1305_block - poly1305 block function
* @param1: struct poly1305_ctx * # poly1305 struct context
* @param2: const uint8 * # input buffer (length: POLY1305_BLOCKSIZE)
* @param3: uint32        # padding bit
* @return: void
*/
void FSYMBOL(poly1305_block)(struct poly1305_ctx *ctx, const uint8 *s,
		uint32 padbit) {
	uint32 h[5], r[5], w[5];
	uint64L d[5];

	for (int32 i = 0; i < 5; i++)
		h[i] = ctx->h[i];
	for (int32 i = 0; i < 5; i++)
		r[i] = ctx->r[i];
	for (int32 i = 0; i < 5; i++)
		w[i] = r[i] * 5;

	/* h = h + m */
	h[0] += PACK4(s) & 0x3ffffff;
	h[1] += (PACK4(s + 3) >> 2) & 0x3ffffff;
	h[2] += (PACK4(s + 6) >> 4) & 0x3ffffff;
	h[3] += (PACK4(s + 9) >> 6) & 0x3ffffff;
	h[4] += (PACK4(s + 12) >> 8) | padbit;

	/* d = h * r */
	d[0] = ((uint64L)h[0] * r[0])
		+ ((uint64L)h[1] * w[4])
		+ ((uint64L)h[2] * w[3])
		+ ((uint64L)h[3] * w[2])
		+ ((uint64L)h[4] * w[1]);
	d[1] = (d[0] >> 26)
		+ ((uint64L)h[0] * r[1])
		+ ((uint64L)h[1] * r[0])
		+ ((uint64L)h[2] * w[4])
		+ ((uint64L)h[3] * w[3])
		+ ((uint64L)h[4] * w[2]);
	d[2] = (d[1] >> 26)
		+ ((uint64L)h[0] * r[2])
		+ ((uint64L)h[1] * r[1])
		+ ((uint64L)h[2] * r[0])
		+ ((uint64L)h[3] * w[4])
		+ ((uint64L)h[4] * w[3]);
	d[3] = (d[2] >> 26)
		+ ((uint64L)h[0] * r[3])
		+ ((uint64L)h[1] * r[2])
		+ ((uint64L)h[2] * r[1])
		+ ((uint64L)h[3] * r[0])
		+ ((uint64L)h[4] * w[4]);
	d[4] = (d[3] >> 26)
		+ ((uint64L)h[0] * r[4])
		+ ((uint64L)h[1] * r[3])
		+ ((uint64L)h[2] * r[2])
		+ ((uint64L)h[3] * r[1])
		+ ((uint64L)h[4] * r[0]);

	/* h = d % p */
	h[0] = (uint32)d[0] & 0x3ffffff;
	h[1] = (uint32)d[1] & 0x3ffffff;
	h[2] = (uint32)d[2] & 0x3ffffff;
	h[3] = (uint32)d[3] & 0x3ffffff;
	h[4] = (uint32)d[4] & 0x3ffffff;

	h[0] += (uint32)(d[4] >> 26) * 5;
	h[1] += h[0] >> 26;
	h[0] = h[0] & 0x3ffffff;

	for (int32 i = 0; i < 5; i++)
		ctx->h[i] = h[i];
} /* end */

/* @func: poly1305_process - poly1305 processing buffer
* @param1: struct poly1305_ctx * # poly1305 struct context
* @param2: const uint8 * # input buffer
* @param3: uint64        # input length
* @return: void
*/
void FSYMBOL(poly1305_process)(struct poly1305_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == POLY1305_BLOCKSIZE) {
			FSYMBOL(poly1305_block)(ctx, ctx->buf, 1 << 24);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: poly1305_finish - poly1305 process the remaining bytes in the \
*                           buffer and end
* @param1: struct poly1305_ctx * # poly1305 struct context
* @return: void
*/
void FSYMBOL(poly1305_finish)(struct poly1305_ctx *ctx) {
	uint32 h[5], t[5];

	if (ctx->count) {
		XSYMBOL(memset)(ctx->buf + ctx->count, 0,
			POLY1305_BLOCKSIZE - ctx->count);
		ctx->buf[ctx->count] = 0x01;
		FSYMBOL(poly1305_block)(ctx, ctx->buf, 0);
	}

	for (int32 i = 0; i < 5; i++)
		h[i] = ctx->h[i];

	/* h = h % p modular reduction */
	h[2] += h[1] >> 26;
	h[1] &= 0x3ffffff;
	h[3] += h[2] >> 26;
	h[2] &= 0x3ffffff;
	h[4] += h[3] >> 26;
	h[3] &= 0x3ffffff;
	h[0] += 5 * (h[4] >> 26);
	h[4] &= 0x3ffffff;
	h[1] += h[0] >> 26;
	h[0] &= 0x3ffffff;

	/* t = h - p */
	t[0] = h[0] + 5;
	t[1] = h[1] + (t[0] >> 26);
	t[2] = h[2] + (t[1] >> 26);
	t[3] = h[3] + (t[2] >> 26);
	t[4] = h[4] + (t[3] >> 26) - (1 << 26);
	t[0] &= 0x3ffffff;
	t[1] &= 0x3ffffff;
	t[2] &= 0x3ffffff;
	t[3] &= 0x3ffffff;

	/* swap */
	uint32 t_mask = (t[4] >> 31) - 1;
	uint32 h_mask = ~t_mask;
	h[0] = (h[0] & h_mask) | (t[0] & t_mask);
	h[1] = (h[1] & h_mask) | (t[1] & t_mask);
	h[2] = (h[2] & h_mask) | (t[2] & t_mask);
	h[3] = (h[3] & h_mask) | (t[3] & t_mask);
	h[4] = (h[4] & h_mask) | (t[4] & t_mask);

	/* h = h % 2**128 */
	h[0] |= h[1] << 26;
	h[1] = (h[1] >> 6) | (h[2] << 20);
	h[2] = (h[2] >> 12) | (h[3] << 14);
	h[3] = (h[3] >> 18) | (h[4] << 8);

	/* tag = (h + s) % 2**128 */
	uint64L tt = (uint64L)h[0] + ctx->s[0];
	h[0] = (uint32)tt;
	tt = (uint64L)h[1] + ctx->s[1] + (tt >> 32);
	h[1] = (uint32)tt;
	tt = (uint64L)h[2] + ctx->s[2] + (tt >> 32);
	h[2] = (uint32)tt;
	tt = (uint64L)h[3] + ctx->s[3] + (tt >> 32);
	h[3] = (uint32)tt;

	for (int32 i = 0; i < 4; i++)
		ctx->tag[i] = h[i];
} /* end */

/* @func: poly1305 - poly1305 processing
* @param1: struct poly1305_ctx * # poly1305 struct context
* @param2: const uint8 * # input buffer
* @param3: uint64        # input length
* @return: void
*/
void FSYMBOL(poly1305)(struct poly1305_ctx *ctx, const uint8 *s, uint64 len) {
	FSYMBOL(poly1305_process)(ctx, s, len);
	FSYMBOL(poly1305_finish)(ctx);
} /* end */
