/* md5.c - md5 hash algorithm implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/md5.h>


/* @def: md5
* md5 round constants
*/
static const uint32 md5_constants[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

/* md5 round number */
static const uint32 md5_number[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};

#undef ROL
#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
/* round functions */
#undef FF
#define FF(b, c, d) (d ^ (b & (c ^ d)))
#undef FG
#define FG(b, c, d) FF(d, b, c)
#undef FH
#define FH(b, c, d) (b ^ c ^ d)
#undef FI
#define FI(b, c, d) (c ^ (b | ~d))
/* end */

/* @func: _md5_compress (static) - md5 compress function
* @param1: struct md5_ctx * # md5 struct context
* @return: void
*/
static void _md5_compress(struct md5_ctx *ctx) {
	uint32 A, B, C, D, m[16];
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];

	for (int32 i = 0; i < 16; i++)
		m[i] = ((uint32 *)ctx->buf)[i];

	uint32 tmp = 0, f = 0, g = 0;
	for (int32 i = 0; i < 64; i++) {
		if (i < 16) {
			f = FF(B, C, D);
			g = i;
		} else if (i < 32) {
			f = FG(B, C, D);
			g = (5 * i + 1) % 16;
		} else if (i < 48) {
			f = FH(B, C, D);
			g = (3 * i + 5) % 16;
		} else {
			f = FI(B, C, D);
			g = (7 * i) % 16;
		}

		tmp = D;
		D = C;
		C = B;
		B = B + ROL(A + f + md5_constants[i] + m[g], md5_number[i]);
		A = tmp;
	}

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
} /* end */

/* @func: md5_init - md5 struct context init
* @param1: struct md5_ctx * # md5 struct context
* @return: void
*/
void FSYMBOL(md5_init)(struct md5_ctx *ctx) {
	ctx->count = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xefcdab89;
	ctx->state[2] = 0x98badcfe;
	ctx->state[3] = 0x10325476;
} /* end */

/* @func: md5_process - md5 processing buffer
* @param1: struct md5_ctx * # md5 struct context
* @param2: const uint8 *    # input buffer
* @param3: uint64           # input length
* @return: void
*/
void FSYMBOL(md5_process)(struct md5_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == 64) {
			_md5_compress(ctx);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: md5_finish - md5 process the remaining bytes in the \
*                      buffer and end
* @param1: struct md5_ctx * # md5 struct context
* @param2: uint64L          # total length
* @return: void
*/
void FSYMBOL(md5_finish)(struct md5_ctx *ctx, uint64L len) {
	/* padding */
	uint8 padbuf[64];
	XSYMBOL(memset)(padbuf, 0, sizeof(padbuf));
	padbuf[0] = 0x80;
	FSYMBOL(md5_process)(ctx, padbuf, 1 + ((119 - (len % 64)) % 64));

	/* bit length */
	((uint64L *)ctx->buf)[7] = len * 8;
	_md5_compress(ctx);
} /* end */

/* @func: md5 - md5 processing
* @param1: struct md5_ctx * # md5 struct context
* @param2: const uint8 *    # input buffer
* @param3: uint64           # input length
* @return: void
*/
void FSYMBOL(md5)(struct md5_ctx *ctx, const uint8 *s, uint64 len) {
	FSYMBOL(md5_process)(ctx, s, len);
	FSYMBOL(md5_finish)(ctx, len);
} /* end */
