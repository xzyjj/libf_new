/* bfifo.c - first-in-first-out (fifo) buffer implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ds/bfifo.h>


/* @func: bfifo_used - get used space in fifo-buffer
* @param1: struct bfifo_ctx * # bfifo struct context
* @return: uint32             # used size
*/
uint32 FSYMBOL(bfifo_used)(struct bfifo_ctx *ctx) {
	return ctx->in - ctx->out;
} /* end */

/* @func: bfifo_avail - get available space in fifo-buffer
* @param1: struct bfifo_ctx * # bfifo struct context
* @return: uint32             # avail size
*/
uint32 FSYMBOL(bfifo_avail)(struct bfifo_ctx *ctx) {
	return ctx->size - (ctx->in - ctx->out);
} /* end */

/* @func: bfifo_is_empty - check if fifo-buffer is empty
* @param1: struct bfifo_ctx * # bfifo struct context
* @return: int32              # 0: non-empy, 1: empty
*/
int32 FSYMBOL(bfifo_is_empty)(struct bfifo_ctx *ctx) {
	return ctx->in == ctx->out;
} /* end */

/* @func: bfifo_is_full - check if fifo-buffer is full
* @param1: struct bfifo_ctx * # bfifo struct context
* @return: int32              # 0: non-full, 1: full
*/
int32 FSYMBOL(bfifo_is_full)(struct bfifo_ctx *ctx) {
	return ctx->size == (ctx->in - ctx->out);
} /* end */

/* @func: bfifo_reset - reset fifo-buffer size
* @param1: struct bfifo_ctx * # bfifo struct context
* @return: void
*/
void FSYMBOL(bfifo_reset)(struct bfifo_ctx *ctx) {
	ctx->in = ctx->out = 0;
} /* end */

/* @func: bfifo_in - put data into fifo-buffer
* @param1: struct bfifo_ctx * # bfifo struct context
* @param2: const void *       # input buffer
* @param3: uint32             # buffer length
* @return: uint32             # input length
*/
uint32 FSYMBOL(bfifo_in)(struct bfifo_ctx *ctx, const void *buf, uint32 len) {
	uint32 l, f;
	l = ctx->size - (ctx->in - ctx->out);
	f = ctx->in % ctx->size;

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	ctx->in += len = MIN(len, l);
	l = MIN(len, ctx->size - f);

	XSYMBOL(memcpy)(ctx->buf + f, buf, l);
	XSYMBOL(memcpy)(ctx->buf, (char *)buf + l, len - l);

	return len;
} /* end */

/* @func: bfifo_out - get data from fifo-buffer
* @param1: struct bfifo_ctx * # bfifo struct context
* @param2: void *             # output buffer
* @param3: uint32             # buffer length
* @return: uint32             # output length
*/
uint32 FSYMBOL(bfifo_out)(struct bfifo_ctx *ctx, void *buf, uint32 len) {
	uint32 l, f;
	l = ctx->in - ctx->out;
	f = ctx->out % ctx->size;

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	ctx->out += len = MIN(len, l);
	l = MIN(len, ctx->size - f);

	XSYMBOL(memcpy)(buf, ctx->buf + f, l);
	XSYMBOL(memcpy)((char *)buf + l, ctx->buf, len - l);

	return len;
} /* end */

/* @func: bfifo_out_peek - peek only at fifo-buffer data
* @param1: struct bfifo_ctx * # bfifo struct context
* @param2: void *             # output buffer
* @param3: uint32             # buffer length
* @param3: uint32             # out offset
* @return: uint32             # output length
*/
uint32 FSYMBOL(bfifo_out_peek)(struct bfifo_ctx *ctx, void *buf, uint32 len,
		uint32 offset) {
	uint32 l = ctx->in - ctx->out;
	uint32 f = (ctx->out + offset) % ctx->size;

	if (offset >= l)
		return 0;

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	len = MIN(len, l);
	l = MIN(len, ctx->size - f);

	XSYMBOL(memcpy)(buf, ctx->buf + f, l);
	XSYMBOL(memcpy)((char *)buf + l, ctx->buf, len - l);

	return len;
} /* end */
