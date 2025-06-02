/* sha512.c - sha512 hash algorithm implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xbswap.h>
#include <libf/al/sha512.h>


/* @def: sha512
* sha512 round constants
*/
static const uint64L sha512_constants[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
	0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
	0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
	0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
	0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
	0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
	0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
	0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
	0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
	0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
	0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
	0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
	0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
	0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
	0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
	0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
	0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
	0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
	0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
	0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
	0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
	};

#undef SROL
#define SROL(a, b) (((a) >> (b)) | ((a) << (64 - (b))))
#undef SHR
#define SHR(a, b) ((a) >> (b))
#undef SCH
#define SCH(a, b, c) (((a) & (b)) ^ (~(a) & (c)))
#undef SMAJ
#define SMAJ(a, b, c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
#undef SIG0
#define SIG0(x) (SROL(x, 28) ^ SROL(x, 34) ^ SROL(x, 39))
#undef SIG1
#define SIG1(x) (SROL(x, 14) ^ SROL(x, 18) ^ SROL(x, 41))
#undef SEP0
#define SEP0(x) (SROL(x, 1) ^ SROL(x, 8) ^ SHR(x, 7))
#undef SEP1
#define SEP1(x) (SROL(x, 19) ^ SROL(x, 61) ^ SHR(x, 6))
/* end */

/* @func: _sha512_compress (static) - sha512 compress function
* @param1: struct sha512_ctx * # sha512 struct context
* @return: void
*/
static void _sha512_compress(struct sha512_ctx *ctx) {
	uint64L A, B, C, D, E, F, G, H, t1, t2, m[80];
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];
	F = ctx->state[5];
	G = ctx->state[6];
	H = ctx->state[7];

	for (int32 i = 0; i < 16; i++)
		m[i] = BSWAP64(((uint64L *)ctx->buf)[i]);
	for (int32 i = 16; i < 80; i++)
		m[i] = SEP1(m[i - 2]) + m[i - 7] + SEP0(m[i - 15]) + m[i - 16];

	for (int32 i = 0; i < 80; i++) {
		t1 = H + SIG1(E) + SCH(E, F, G) + sha512_constants[i] + m[i];
		t2 = SIG0(A) + SMAJ(A, B, C);
		H = G;
		G = F;
		F = E;
		E = D + t1;
		D = C;
		C = B;
		B = A;
		A = t1 + t2;
	}

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
	ctx->state[4] += E;
	ctx->state[5] += F;
	ctx->state[6] += G;
	ctx->state[7] += H;
} /* end */

/* @func: sha512_init - sha512 struct context init
* @param1: struct sha512_ctx * # sha512 struct context
* @return: void
*/
void FSYMBOL(sha512_init)(struct sha512_ctx *ctx) {
	ctx->count = 0;
	ctx->state[0] = 0x6a09e667f3bcc908ULL;
	ctx->state[1] = 0xbb67ae8584caa73bULL;
	ctx->state[2] = 0x3c6ef372fe94f82bULL;
	ctx->state[3] = 0xa54ff53a5f1d36f1ULL;
	ctx->state[4] = 0x510e527fade682d1ULL;
	ctx->state[5] = 0x9b05688c2b3e6c1fULL;
	ctx->state[6] = 0x1f83d9abfb41bd6bULL;
	ctx->state[7] = 0x5be0cd19137e2179ULL;
} /* end */

/* @func: sha384_init - sha384 struct context init
* @param1: struct sha384_ctx * # sha384 struct context
* @return: void
*/
void FSYMBOL(sha384_init)(struct sha512_ctx *ctx) {
	ctx->count = 0;
	ctx->state[0] = 0xcbbb9d5dc1059ed8ULL;
	ctx->state[1] = 0x629a292a367cd507ULL;
	ctx->state[2] = 0x9159015a3070dd17ULL;
	ctx->state[3] = 0x152fecd8f70e5939ULL;
	ctx->state[4] = 0x67332667ffc00b31ULL;
	ctx->state[5] = 0x8eb44a8768581511ULL;
	ctx->state[6] = 0xdb0c2e0d64f98fa7ULL;
	ctx->state[7] = 0x47b5481dbefa4fa4ULL;
} /* end */

/* @func: sha512_process - sha512 processing buffer
* @param1: struct sha512_ctx * # sha512 struct context
* @param2: const uint8 *       # input buffer
* @param3: uint64              # input length
* @return: void
*/
void FSYMBOL(sha512_process)(struct sha512_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == 128) {
			_sha512_compress(ctx);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: sha512_finish - sha512 process the remaining bytes in the \
*                         buffer and end
* @param1: struct sha515_ctx * # sha512 struct context
* @param2: uint64L             # total length
* @return: void
*/
void FSYMBOL(sha512_finish)(struct sha512_ctx *ctx, uint64L len) {
	/* padding */
	uint8 padbuf[128];
	XSYMBOL(memset)(padbuf, 0, sizeof(padbuf));
	padbuf[0] = 0x80;
	FSYMBOL(sha512_process)(ctx, padbuf, 1 + ((239 - (len % 128)) % 128));

	/* bit length */
	((uint64L *)ctx->buf)[14] = 0;
	((uint64L *)ctx->buf)[15] = BSWAP64(len * 8);
	_sha512_compress(ctx);

	for (int32 i = 0; i < 8; i++)
		ctx->state[i] = BSWAP64(ctx->state[i]);
} /* end */

/* @func: sha512 - sha512 processing
* @param1: struct sha512_ctx * # sha512 struct context
* @param2: const uint8 *       # input buffer
* @param3: uint64              # input length
* @return: void
*/
void FSYMBOL(sha512)(struct sha512_ctx *ctx, const uint8 *s, uint64 len) {
	ctx->count = 0;
	FSYMBOL(sha512_process)(ctx, s, len);
	FSYMBOL(sha512_finish)(ctx, len);
} /* end */
