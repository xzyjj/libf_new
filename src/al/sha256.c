/* sha256.c - sha256 hash algorithm implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xbswap.h>
#include <libf/al/sha256.h>


/* @def: sha256
* sha256 round constants
*/
static const uint32 sha256_constants[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
	};

#undef ROL
#define ROL(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
#undef CH
#define CH(a, b, c) (((a) & (b)) ^ (~(a) & (c)))
#undef MAJ
#define MAJ(a, b, c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
#undef EP0
#define EP0(x) (ROL(x, 2) ^ ROL(x, 13) ^ ROL(x, 22))
#undef EP1
#define EP1(x) (ROL(x, 6) ^ ROL(x, 11) ^ ROL(x, 25))
#undef SIG0
#define SIG0(x) (ROL(x, 7) ^ ROL(x, 18) ^ ((x) >> 3))
#undef SIG1
#define SIG1(x) (ROL(x, 17) ^ ROL(x, 19) ^ ((x) >> 10))
/* end */

/* @func: _sha256_compress (static) - sha256 compress function
* @param1: struct sha256_ctx * # sha256 struct context
* @return: void
*/
static void _sha256_compress(struct sha256_ctx *ctx) {
	uint32 A, B, C, D, E, F, G, H, t1, t2, m[64];
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];
	F = ctx->state[5];
	G = ctx->state[6];
	H = ctx->state[7];

	for (int32 i = 0; i < 16; i++)
		m[i] = BSWAP32(((uint32 *)ctx->buf)[i]);
	for (int32 i = 16; i < 64; i++)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	for (int32 i = 0; i < 64; i++) {
		t1 = H + EP1(E) + CH(E, F, G) + sha256_constants[i] + m[i];
		t2 = EP0(A) + MAJ(A, B, C);
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

/* @func: sha256_init - sha256 struct context init
* @param1: struct sha256_ctx * # sha256 struct context
* @return: void
*/
void FSYMBOL(sha256_init)(struct sha256_ctx *ctx) {
	ctx->count = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
} /* end */

/* @func: sha224_init - sha224 struct context init
* @param1: struct sha224_ctx * # sha224 struct context
* @return: void
*/
void FSYMBOL(sha224_init)(struct sha256_ctx *ctx) {
	ctx->count = 0;
	ctx->state[0] = 0xc1059ed8;
	ctx->state[1] = 0x367cd507;
	ctx->state[2] = 0x3070dd17;
	ctx->state[3] = 0xf70e5939;
	ctx->state[4] = 0xffc00b31;
	ctx->state[5] = 0x68581511;
	ctx->state[6] = 0x64f98fa7;
	ctx->state[7] = 0xbefa4fa4;
} /* end */

/* @func: sha256_process - sha256 processing buffer
* @param1: struct sha256_ctx * # sha256 struct context
* @param2: conat uint8 *       # input buffer
* @param3: uint64              # input length
* @return: void
*/
void FSYMBOL(sha256_process)(struct sha256_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == SHA256_BLOCKSIZE) {
			_sha256_compress(ctx);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: sha256_finish - sha256 process the remaining bytes in the \
*                         buffer and end
* @param1: struct sha256_ctx * # sha256 struct context
* @param2: uint64L             # total length
* @return: void
*/
void FSYMBOL(sha256_finish)(struct sha256_ctx *ctx, uint64L len) {
	/* padding */
	uint8 padbuf[SHA256_BLOCKSIZE];
	XSYMBOL(memset)(padbuf, 0, sizeof(padbuf));
	padbuf[0] = 0x80;
	FSYMBOL(sha256_process)(ctx, padbuf, 1 + ((119 - (len % 64)) % 64));

	/* bit length */
	((uint64L *)ctx->buf)[7] = BSWAP64(len * 8);
	_sha256_compress(ctx);

	for (int32 i = 0; i < 8; i++)
		ctx->state[i] = BSWAP32(ctx->state[i]);
} /* end */

/* @func: sha256 - sha256 processing
* @param1: struct sha256_ctx * # sha256 struct context
* @param2: const uint8 *       # input buffer
* @param3: uint64              # input length
* @return: void
*/
void FSYMBOL(sha256)(struct sha256_ctx *ctx, const uint8 *s, uint64 len) {
	FSYMBOL(sha256_process)(ctx, s, len);
	FSYMBOL(sha256_finish)(ctx, len);
} /* end */
