/* sha1.c - sha1 hash algorithm implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xbswap.h>
#include <libf/al/sha1.h>


/* @def: sha1 */
#undef ROL
#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/* round functions */
#undef F1
#define F1(b, c, d) (d ^ (b & (c ^ d)))
#undef F2
#define F2(b, c, d) (b ^ c ^ d)
#undef F3
#define F3(b, c, d) ((b & c) | (d & (b | c)))
#undef F4
#define F4(b, c, d) (b ^ c ^ d)

/* round constants */
#undef K1
#define K1 0x5a827999
#undef K2
#define K2 0x6ed9eba1
#undef K3
#define K3 0x8f1bbcdc
#undef K4
#define K4 0xca62c1d6
/* end */

/* @func: _sha1_compress (static) - sha1 compress function
* @param1: struct sha1_ctx * # sha1 struct context
* @return: void
*/
static void _sha1_compress(struct sha1_ctx *ctx) {
	uint32 A, B, C, D, E, m[80];
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];

	for (int32 i = 0; i < 16; i++)
		m[i] = BSWAP32(((uint32 *)ctx->buf)[i]);
	for (int32 i = 16; i < 80; i++)
		m[i] = ROL(m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16], 1);

	for (int32 i = 0, tmp = 0; i < 80; i++) {
		if (i < 20) {
			tmp = ROL(A, 5) + F1(B, C, D) + E + K1 + m[i];
		} else if (i < 40) {
			tmp = ROL(A, 5) + F2(B, C, D) + E + K2 + m[i];
		} else if (i < 60) {
			tmp = ROL(A, 5) + F3(B, C, D) + E + K3 + m[i];
		} else {
			tmp = ROL(A, 5) + F4(B, C, D) + E + K4 + m[i];
		}

		E = D;
		D = C;
		C = ROL(B, 30);
		B = A;
		A = tmp;
	}

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
	ctx->state[4] += E;
} /* end */

/* @func: sha1_init - sha1 struct context init
* @param1: struct sha1_ctx * # sha1 struct context
* @return: void
*/
void FSYMBOL(sha1_init)(struct sha1_ctx *ctx) {
	ctx->count = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xefcdab89;
	ctx->state[2] = 0x98badcfe;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xc3d2e1f0;
} /* end */

/* @func: sha1_process - sha1 processing buffer
* @param1: struct sha1_ctx * # sha1 struct context
* @param2: const uint8 *     # input buffer
* @param3: uint64            # input length
* @return: void
*/
void FSYMBOL(sha1_process)(struct sha1_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == 64) {
			_sha1_compress(ctx);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: sha1_finish - sha1 process the remaining bytes in the \
*                       buffer and end
* @param1: struct sha1_ctx * # sha1 struct context
* @param2: uint64L           # total length
* @return: void
*/
void FSYMBOL(sha1_finish)(struct sha1_ctx *ctx, uint64L len) {
	/* padding */
	uint8 padbuf[64];
	XSYMBOL(memset)(padbuf, 0, sizeof(padbuf));
	padbuf[0] = 0x80;
	FSYMBOL(sha1_process)(ctx, padbuf,
		1 + ((119 - (len % 64)) % 64));

	/* bit length */
	((uint64L *)ctx->buf)[7] = BSWAP64(len * 8);
	_sha1_compress(ctx);

	for (int32 i = 0; i < 5; i++)
		ctx->state[i] = BSWAP32(ctx->state[i]);
} /* end */

/* @func: sha1 - sha1 processing
* @param1: struct sha1_ctx * # sha1 struct context
* @param2: const uint8 *     # input buffer
* @param3: uint64            # input length
* @return: void
*/
void FSYMBOL(sha1)(struct sha1_ctx *ctx, const uint8 *s, uint64 len) {
	ctx->count = 0;
	FSYMBOL(sha1_process)(ctx, s, len);
	FSYMBOL(sha1_finish)(ctx, len);
} /* end */
