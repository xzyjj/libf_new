/* xxhash64.c - extremely fast hash algorithm (64bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/xxhash64.h>


/* @def: xxhash64 */
#undef PRIME64_1
#define PRIME64_1 0x9e3779b185ebca87ULL
#undef PRIME64_2
#define PRIME64_2 0xc2b2ae3d27d4eb4fULL
#undef PRIME64_3
#define PRIME64_3 0x165667b19e3779f9ULL
#undef PRIME64_4
#define PRIME64_4 0x85ebca77c2b2ae63ULL
#undef PRIME64_5
#define PRIME64_5 0x27d4eb2f165667c5ULL

#undef ROTL64
#define ROTL64(x, n) (((x) << (n)) | ((x) >> (64 - (n))))
/* end */

/* @func: _xxhash64_compress (static) - xxh64 compress function
* @param1: struct xxhash64_ctx * # xxh64 struct context
* @return: void
*/
static void _xxhash64_compress(struct xxhash64_ctx *ctx) {
	uint64L a = ctx->state[0];
	uint64L b = ctx->state[1];
	uint64L c = ctx->state[2];
	uint64L d = ctx->state[3];

	/* round */
	a += ((uint64L *)ctx->buf)[0] * PRIME64_2;
	a = ROTL64(a, 31) * PRIME64_1;
	b += ((uint64L *)ctx->buf)[1] * PRIME64_2;
	b = ROTL64(b, 31) * PRIME64_1;
	c += ((uint64L *)ctx->buf)[2] * PRIME64_2;
	c = ROTL64(c, 31) * PRIME64_1;
	d += ((uint64L *)ctx->buf)[3] * PRIME64_2;
	d = ROTL64(d, 31) * PRIME64_1;

	ctx->state[0] = a;
	ctx->state[1] = b;
	ctx->state[2] = c;
	ctx->state[3] = d;
} /* end */

/* @func: xxhash64_init - xxh64 init function
* @param1: struct xxhash64_ctx * # xxh64 struct context
* @param2: uint64L               # init value (default: 0)
* @return: void
*/
void FSYMBOL(xxhash64_init)(struct xxhash64_ctx *ctx, uint64L seed) {
	ctx->seed = seed;
	ctx->state[0] = seed + PRIME64_1 + PRIME64_2;
	ctx->state[1] = seed + PRIME64_2;
	ctx->state[2] = seed;
	ctx->state[3] = seed - PRIME64_1;
	ctx->count = 0;
} /* end */

/* @func: xxhash64_process - xxh64 processing buffer
* @param1: struct xxhash64_ctx * # xxh64 struct context
* @param2: const uint8 *         # input buffer
* @param3: uint64                # input length
* @return: void
*/
void FSYMBOL(xxhash64_process)(struct xxhash64_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == XXHASH64_BLOCKSIZE) {
			_xxhash64_compress(ctx);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: xxhash64_finish - xxh64 process the remaining bytes in the \
*                           buffer and end
* @param1: struct xxhash64_ctx * # xxh64 struct context
* @param2: uint64L               # total length
* @return: uint64L               # return hash digest
*/
uint64L FSYMBOL(xxhash64_finish)(struct xxhash64_ctx *ctx, uint64L len) {
	uint64L a = ctx->state[0];
	uint64L b = ctx->state[1];
	uint64L c = ctx->state[2];
	uint64L d = ctx->state[3];
	uint64L hash = 0;
	uint8 *p = ctx->buf;

	if (len < XXHASH64_BLOCKSIZE) {
		hash = ctx->seed + PRIME64_5;
	} else {
		hash = ROTL64(a, 1) + ROTL64(b, 7)
			+ ROTL64(c, 12) + ROTL64(d, 18);

		/* merge round */
		a *= PRIME64_2;
		hash ^= ROTL64(a, 31) * PRIME64_1;
		hash = (hash * PRIME64_1) + PRIME64_4;

		b *= PRIME64_2;
		hash ^= ROTL64(b, 31) * PRIME64_1;
		hash = (hash * PRIME64_1) + PRIME64_4;

		c *= PRIME64_2;
		hash ^= ROTL64(c, 31) * PRIME64_1;
		hash = (hash * PRIME64_1) + PRIME64_4;

		d *= PRIME64_2;
		hash ^= ROTL64(d, 31) * PRIME64_1;
		hash = (hash * PRIME64_1) + PRIME64_4;
	}
	hash += len;

	/* remaining processing */
	while (ctx->count > 7) {
		a = *((uint64L *)p) * PRIME64_2;
		hash ^= ROTL64(a, 31) * PRIME64_1;
		hash = ROTL64(hash, 27) * PRIME64_1;
		hash += PRIME64_4;
		p += 8;
		ctx->count -= 8;
	}
	while (ctx->count > 3) {
		a = *((uint32 *)p);
		hash ^= a * PRIME64_1;
		hash = ROTL64(hash, 23) * PRIME64_2;
		hash += PRIME64_3;
		p += 4;
		ctx->count -= 4;
	}
	while (ctx->count--) {
		a = *p++;
		hash ^= a * PRIME64_5;
		hash = ROTL64(hash, 11) * PRIME64_1;
	}

	/* final mix */
	hash ^= hash >> 33;
	hash *= PRIME64_2;
	hash ^= hash >> 29;
	hash *= PRIME64_3;
	hash ^= hash >> 32;

	return hash;
} /* end */

/* @func: xxhash64 - xxh64 processing
* @param1: struct xxhash64_ctx * # xxh64 struct context
* @param2: const uint8 *         # input buffer
* @param3: uint64                # input length
* @return: uint64L               # return hash digest
*/
uint64L FSYMBOL(xxhash64)(struct xxhash64_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(xxhash64_process)(ctx, s, len);
	return FSYMBOL(xxhash64_finish)(ctx, len);
} /* end */
