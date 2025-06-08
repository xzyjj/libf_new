/* blake2s.c - blake2 cryptographic hash implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/blake2s.h>


/* @def: blake2s
* initialization vector */
static const uint32 blake2s_iv[8] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

/* sigma substitution table */
static const uint8 blake2s_sigma[12][16] = {
	{  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
	{ 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 },
	{ 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 },
	{  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 },
	{  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 },
	{  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 },
	{ 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 },
	{ 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 },
	{  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 },
	{ 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13 , 0 },
	{  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
	{ 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 }
	};

#undef ROTR32
#define ROTR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

/* mixing function */
#undef BLAKE2S_G
#define BLAKE2S_G(v, m, a, b, c, d, x, y) \
	v[a] = v[a] + v[b] + m[x]; \
	v[d] = ROTR32(v[d] ^ v[a], 16); \
	v[c] = v[c] + v[d]; \
	v[b] = ROTR32(v[b] ^ v[c], 12); \
	v[a] = v[a] + v[b] + m[y]; \
	v[d] = ROTR32(v[d] ^ v[a], 8); \
	v[c] = v[c] + v[d]; \
	v[b] = ROTR32(v[b] ^ v[c], 7)
/* end */

/* @func: _blake2s_compress (static) - blake2s compress function
* @param1: struct blake2s_ctx # blake2s struct context
* @return: void
*/
static void _blake2s_compress(struct blake2s_ctx *ctx) {
	uint32 m[16], v[16];

	for (int32 i = 0; i < 16; i++)
		m[i] = ((uint32 *)ctx->buf)[i];
	for (int32 i = 0; i < 8; i++) {
		v[i] = ctx->state[i];
		v[i + 8] = blake2s_iv[i];
	}

	v[12] ^= ctx->tsize[0];
	v[13] ^= ctx->tsize[1];
	v[14] ^= ctx->flags[0];
	v[15] ^= ctx->flags[1];

	for (int32 i = 0; i < 10; i++) {
		BLAKE2S_G(v, m, 0, 4, 8, 12,
			blake2s_sigma[i][0], blake2s_sigma[i][1]);
		BLAKE2S_G(v, m, 1, 5, 9, 13,
			blake2s_sigma[i][2], blake2s_sigma[i][3]);
		BLAKE2S_G(v, m, 2, 6, 10, 14,
			blake2s_sigma[i][4], blake2s_sigma[i][5]);
		BLAKE2S_G(v, m, 3, 7, 11, 15,
			blake2s_sigma[i][6], blake2s_sigma[i][7]);
		BLAKE2S_G(v, m, 0, 5, 10, 15,
			blake2s_sigma[i][8], blake2s_sigma[i][9]);
		BLAKE2S_G(v, m, 1, 6, 11, 12,
			blake2s_sigma[i][10], blake2s_sigma[i][11]);
		BLAKE2S_G(v, m, 2, 7, 8, 13,
			blake2s_sigma[i][12], blake2s_sigma[i][13]);
		BLAKE2S_G(v, m, 3, 4, 9, 14,
			blake2s_sigma[i][14], blake2s_sigma[i][15]);
	}

	for (int32 i = 0; i < 8; i++)
		ctx->state[i] ^= v[i] ^ v[i + 8];
} /* end */

/* @func: blake2s_init - blake2s init function
* @param1: struct blake2s_ctx # blake2s struct context
* @param2: uint32             # digest length (max: BLAKE2S_256_LEN)
* @return: int32              # 0: no error, -1: digest size error
*/
int32 FSYMBOL(blake2s_init)(struct blake2s_ctx *ctx, uint32 dsize) {
	if (!dsize || dsize > BLAKE2S_256_LEN)
		return -1;

	XSYMBOL(memcpy)(ctx->state, blake2s_iv, sizeof(ctx->state));
	ctx->tsize[0] = 0;
	ctx->tsize[1] = 0;
	ctx->flags[0] = 0;
	ctx->flags[1] = 0;
	ctx->count = 0;

	/* default parameters */
	XSYMBOL(memset)(&ctx->u, 0, sizeof(ctx->u));
	ctx->u.param.digest_length = dsize;
	ctx->u.param.fanout = 1;
	ctx->u.param.depth = 1;
	for (int32 i = 0; i < 8; i++)
		ctx->state[i] ^= ctx->u.state[i];

	return 0;
} /* end */

/* @func: blake2s_process - blake2s processing buffer
* @param1: struct blake2s_ctx # blake2s struct context
* @param2: const uint8 *      # input buffer
* @param3: uint64             # input length
* @return: void
*/
void FSYMBOL(blake2s_process)(struct blake2s_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		if (n == BLAKE2S_BLOCKSIZE) {
			ctx->tsize[0] += BLAKE2S_BLOCKSIZE;
			if (ctx->tsize[0] < ctx->count)
				ctx->tsize[1]++;
			_blake2s_compress(ctx);
			n = 0;
		}
		ctx->buf[n++] = s[i];
	}
	ctx->count = n;
} /* end */

/* @func: blake2s_finish - blake2s process the remaining bytes in the \
*                          buffer and end
* @param1: struct blake2s_ctx * # blake2s struct context
* @return: void
*/
void FSYMBOL(blake2s_finish)(struct blake2s_ctx *ctx) {
	XSYMBOL(memset)(ctx->buf + ctx->count, 0,  BLAKE2S_BLOCKSIZE
		- ctx->count);
	ctx->flags[0] = (uint32)-1;
	ctx->tsize[0] += ctx->count;
	if (ctx->tsize[0] < ctx->count)
		ctx->tsize[1]++;
	_blake2s_compress(ctx);
} /* end */

/* @func: blake2s - blake2s processing
* @param1: struct blake2s_ctx * # blake2s struct context
* @param2: const uint8 *        # input buffer
* @param3: uint64               # input length
* @return: void
*/
void FSYMBOL(blake2s)(struct blake2s_ctx *ctx, const uint8 *s, uint64 len) {
	FSYMBOL(blake2s_process)(ctx, s, len);
	FSYMBOL(blake2s_finish)(ctx);
} /* end */
