/* xxhash32.c - extremely fast hash algorithm (32bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/xxhash32.h>


/* @def: xxhash32 */
#undef PRIME32_1
#define PRIME32_1 0x9e3779b1
#undef PRIME32_2
#define PRIME32_2 0x85ebca77
#undef PRIME32_3
#define PRIME32_3 0xc2b2ae3d
#undef PRIME32_4
#define PRIME32_4 0x27d4eb2f
#undef PRIME32_5
#define PRIME32_5 0x165667b1

#undef ROTL32
#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
/* end */

/* @func: _xxhash32_compress (static) - xxh32 compress function
* @param1: struct xxhash32_ctx * # xxh32 struct context
* @return: void
*/
static void _xxhash32_compress(struct xxhash32_ctx *ctx) {
	uint32 a = ctx->state[0];
	uint32 b = ctx->state[1];
	uint32 c = ctx->state[2];
	uint32 d = ctx->state[3];

	/* round */
	a += ((uint32 *)ctx->buf)[0] * PRIME32_2;
	a = ROTL32(a, 13) * PRIME32_1;
	b += ((uint32 *)ctx->buf)[1] * PRIME32_2;
	b = ROTL32(b, 13) * PRIME32_1;
	c += ((uint32 *)ctx->buf)[2] * PRIME32_2;
	c = ROTL32(c, 13) * PRIME32_1;
	d += ((uint32 *)ctx->buf)[3] * PRIME32_2;
	d = ROTL32(d, 13) * PRIME32_1;

	ctx->state[0] = a;
	ctx->state[1] = b;
	ctx->state[2] = c;
	ctx->state[3] = d;
} /* end */

/* @func: xxhash32_init - xxh32 init function
* @param1: struct xxhash32_ctx * # xxh64 struct context
* @param2: uint32                # init value (default: 0)
* @return: void
*/
void FSYMBOL(xxhash32_init)(struct xxhash32_ctx *ctx, uint32 seed) {
	ctx->seed = seed;
	ctx->state[0] = seed + PRIME32_1 + PRIME32_2;
	ctx->state[1] = seed + PRIME32_2;
	ctx->state[2] = seed;
	ctx->state[3] = seed - PRIME32_1;
	ctx->count = 0;
} /* end */

/* @func: xxhash32_process - xxh32 processing buffer
* @param1: struct xxhash32_ctx * # xxh32 struct context
* @param2: const uint8 *         # input buffer
* @param3: uint64                # input length
* @return: void
*/
void FSYMBOL(xxhash32_process)(struct xxhash32_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == XXHASH32_BLOCKSIZE) {
			_xxhash32_compress(ctx);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: xxhash32_finish - xxh32 process the remaining bytes in the \
*                           buffer and end
* @param1: struct xxhash32_ctx * # xxh64 struct context
* @param2: uint64                # total length
* @return: uint32                # return hash value
*/
uint32 FSYMBOL(xxhash32_finish)(struct xxhash32_ctx *ctx, uint64 len) {
	uint32 a = ctx->state[0];
	uint32 b = ctx->state[1];
	uint32 c = ctx->state[2];
	uint32 d = ctx->state[3];
	uint32 hash = 0;
	uint8 *p = ctx->buf;

	if (len < XXHASH32_BLOCKSIZE) {
		hash = ctx->seed + PRIME32_5;
	} else {
		hash = ROTL32(a, 1) + ROTL32(b, 7)
			+ ROTL32(c, 12) + ROTL32(d, 18);
	}
	hash += (uint32)len;

	/* remaining processing */
	while (ctx->count > 3) {
		a = *((uint32 *)p);
		hash += a * PRIME32_3;
		hash = ROTL32(hash, 17) * PRIME32_4;
		p += 4;
		ctx->count -= 4;
	}
	while (ctx->count--) {
		a = *p++;
		hash += a * PRIME32_5;
		hash = ROTL32(hash, 11) * PRIME32_1;
	}

	/* final mix */
	hash ^= hash >> 15;
	hash *= PRIME32_2;
	hash ^= hash >> 13;
	hash *= PRIME32_3;
	hash ^= hash >> 16;

	return hash;
} /* end */

/* @func: xxhash32 - xxh32 processing
* @param1: struct xxhash32_ctx * # xxh32 struct context
* @param2: const uint8 *         # input buffer
* @param3: uint64                # input length
* @return: void
*/
void FSYMBOL(xxhash32)(struct xxhash32_ctx *ctx, const uint8 *s, uint64 len) {
	FSYMBOL(xxhash32_process)(ctx, s, len);
	FSYMBOL(xxhash32_finish)(ctx, len);
} /* end */
