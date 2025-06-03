/* sha3_fast.c - secure hash-3 algorithm implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/sha3.h>


/* @def: sha3
* iota round constant */
static const uint64L keccak_rndc[24] = {
	0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
	0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
	0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
	0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
	0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
	0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
	0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
	0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
	};

/* rotation offset */
static const uint8 keccak_rho[5][5] = {
	{  0,  1, 62, 28, 27 },
	{ 36, 44,  6, 55, 20 },
	{  3, 10, 43, 25, 39 },
	{ 41, 45, 15, 21,  8 },
	{ 18,  2, 61, 56, 14 }
	};

#undef ROTL64
#define ROTL64(x, n) ((x << n) | (x >> (64 - n)))
/* end */

/* @func: _keccak_f1600 (static) - keccak-f permutation function
* @param1: uint64L [5][5] # state buffer
* @return: void
*/
static void _keccak_f1600(uint64L state[5][5]) {
	uint64L C[5], D[5], B[5][5];

	for (int32 i = 0; i < SHA3_KECCAK_ROUNDS; i++) {
		/* theta */
#undef THETA_1
#define THETA_1(t, s, x) \
	t[x] = s[0][x] \
		^ s[1][x] \
		^ s[2][x] \
		^ s[3][x] \
		^ s[4][x]

#if 1

		THETA_1(C, state,  0);
		THETA_1(C, state,  1);
		THETA_1(C, state,  2);
		THETA_1(C, state,  3);
		THETA_1(C, state,  4);

#else

		for (int32 x = 0; x < 5; x++) {
			C[x] = 0;
			for (int32 y = 0; y < 5; y++)
				C[x] ^= state[y][x];
		}

#endif

#undef THETA_2
#define THETA_2(t, s, x, c0, c1) \
	t[x] = ROTL64(s[c0], 1) ^ s[c1];

#if 1

		THETA_2(D, C, 0, 1, 4);
		THETA_2(D, C, 1, 2, 0);
		THETA_2(D, C, 2, 3, 1);
		THETA_2(D, C, 3, 4, 2);
		THETA_2(D, C, 4, 0, 3);

#else

		for (int32 x = 0; x < 5; x++) {
			D[x] = ROTL64(C[(x + 1) % 5], 1) ^ C[(x + 4) % 5];
		}

#endif

#undef THETA_3
#define THETA_3(t, s, x) \
	t[0][x] ^= s[x]; \
	t[1][x] ^= s[x]; \
	t[2][x] ^= s[x]; \
	t[3][x] ^= s[x]; \
	t[4][x] ^= s[x]

#if 1

		THETA_3(state, D, 0);
		THETA_3(state, D, 1);
		THETA_3(state, D, 2);
		THETA_3(state, D, 3);
		THETA_3(state, D, 4);

#else

		for (int32 x = 0; x < 5; x++) {
			for (int32 y = 0; y < 5; y++)
				state[y][x] ^= D[x];
		}

#endif

		/* rho */
#if 1

		for (int32 y = 0; y < 5; y++) {
			state[y][0] = ROTL64(state[y][0], keccak_rho[y][0]);
			state[y][1] = ROTL64(state[y][1], keccak_rho[y][1]);
			state[y][2] = ROTL64(state[y][2], keccak_rho[y][2]);
			state[y][3] = ROTL64(state[y][3], keccak_rho[y][3]);
			state[y][4] = ROTL64(state[y][4], keccak_rho[y][4]);
		}

#else

		for (int32 x = 0; x < 5; x++) {
			for (int32 y = 0; y < 5; y++)
				state[y][x] = ROTL64(state[y][x],
					keccak_rho[y][x]);
		}

#endif

		/* pi */
		for (int32 x = 0; x < 5; x++) {
			for (int32 y = 0; y < 5; y++)
				B[y][x] = state[y][x];
		}

#undef PI_1
#define PI_1(t, s, y, c0, c1, c2, c3, c4) \
	t[y][0] = s[0][c0]; \
	t[y][1] = s[1][c1]; \
	t[y][2] = s[2][c2]; \
	t[y][3] = s[3][c3]; \
	t[y][4] = s[4][c4]

#if 1

		PI_1(state, B, 0, 0, 1, 2, 3, 4);
		PI_1(state, B, 1, 3, 4, 0, 1, 2);
		PI_1(state, B, 2, 1, 2, 3, 4, 0);
		PI_1(state, B, 3, 4, 0, 1, 2, 3);
		PI_1(state, B, 4, 2, 3, 4, 0, 1);

#else

		for (int32 x = 0; x < 5; x++) {
			for (int32 y = 0; y < 5; y++)
				state[y][x] = B[x][((3 * y) + x) % 5];
		}

#endif

		/* chi */
#undef CHI_1
#define CHI_1(t, s, y) \
	t[0] = s[y][0] ^ (~s[y][1] & s[y][2]); \
	t[1] = s[y][1] ^ (~s[y][2] & s[y][3]); \
	t[2] = s[y][2] ^ (~s[y][3] & s[y][4]); \
	t[3] = s[y][3] ^ (~s[y][4] & s[y][0]); \
	t[4] = s[y][4] ^ (~s[y][0] & s[y][1])

#undef CHI_2
#define CHI_2(t, s, y) \
	t[y][0] = s[0]; \
	t[y][1] = s[1]; \
	t[y][2] = s[2]; \
	t[y][3] = s[3]; \
	t[y][4] = s[4]

#if 0

		for (int32 y = 0; y < 5; y++) {
			CHI_1(C, state, y);
			CHI_2(state, C, y);
		}

#else

		for (int32 y = 0; y < 5; y++) {
			for (int32 x = 0; x < 5; x++)
				C[x] = state[y][x] ^ (~state[y][(x + 1) % 5]
					& state[y][(x + 2) % 5]);
			for (int32 x = 0; x < 5; x++)
				state[y][x] = C[x];
		}

#endif

		/* iota */
		state[0][0] ^= keccak_rndc[i];
	}
} /* end */

/* @func: _keccak_absorb (static) - keccak absorb function
* @param1: uint64L [5][5] # state buffer
* @param2: const uint8 *  # input buffer
* @param3: uint32         # input length
* @param4: uint32         # bitrate length (byte)
* @return: uint32         # remaining length
*/
static uint32 _keccak_absorb(uint64L state[5][5], const uint8 *in,
		uint32 len, uint32 rate) {
	while (len >= rate) {
		for (uint32 i = 0; i < (rate / 8); i++) {
			((uint64L *)state)[i] ^= (uint64L)in[0]
				| (uint64L)in[1] << 8
				| (uint64L)in[2] << 16
				| (uint64L)in[3] << 24
				| (uint64L)in[4] << 32
				| (uint64L)in[5] << 40
				| (uint64L)in[6] << 48
				| (uint64L)in[7] << 56;
			in += 8;
		}
		len -= rate;

		_keccak_f1600(state);
	}

	return len;
} /* end */

/* @func: _keccak_squeeze (static) - keccak squeeze function
* @param1: uint64L [5][5] # state buffer
* @param2: uint8 *        # digest output
* @param3: uint32         # digest length
* @param4: uint32         # bitrate length (byte)
* @return: void
*/
static void _keccak_squeeze(uint64L state[5][5], uint8 *out, uint32 len,
		uint32 rate) {
	while (len) {
		for (uint32 i = 0; i < rate && len; i++) {
			uint64L A = ((uint64L *)state)[i];
			if (len < 8) { /* len < rste */
				for (uint32 k = 0; k < len; k++) {
					*out++ = (uint8)A;
					A >>= 8;
				}
				return;
			}

			out[0] = (uint8)A;
			out[1] = (uint8)(A >> 8);
			out[2] = (uint8)(A >> 16);
			out[3] = (uint8)(A >> 24);
			out[4] = (uint8)(A >> 32);
			out[5] = (uint8)(A >> 40);
			out[6] = (uint8)(A >> 48);
			out[7] = (uint8)(A >> 56);
			out += 8;
			len -= 8;
		}
		if (!len)
			return;

		/* len > rate */
		_keccak_f1600(state);
	}
} /* end */

/* @func: sha3_init - sha3 init function
* @param1: struct sha3_ctx # sha3 struct context
* @return: int32           # 0: no error, -1: type error
*/
int32 FSYMBOL(sha3_init)(struct sha3_ctx *ctx, int32 type) {
	if (type == SHA3_224_TYPE) {
		ctx->rate = SHA3_224_RATE;
		ctx->dsize = SHA3_224_LEN;
	} else if (type == SHA3_256_TYPE) {
		ctx->rate = SHA3_256_RATE;
		ctx->dsize = SHA3_256_LEN;
	} else if (type == SHA3_384_TYPE) {
		ctx->rate = SHA3_384_RATE;
		ctx->dsize = SHA3_384_LEN;
	} else if (type == SHA3_512_TYPE) {
		ctx->rate = SHA3_512_RATE;
		ctx->dsize = SHA3_512_LEN;
	} else {
		return -1;
	}

	XSYMBOL(memset)(ctx->state, 0, SHA3_STATE_SIZE);
	ctx->count = 0;

	return 0;
} /* end */

/* @func: sha3_process - sha3 processing buffer
* @param1: struct sha3_ctx # sha3 struct context
* @param2: const uint8 *       # input buffer
* @param3: uint64              # input length
* @return: void
*/
void FSYMBOL(sha3_process)(struct sha3_ctx *ctx, const uint8 *s,
		uint64 len) {
	uint32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == ctx->rate) {
			_keccak_absorb(ctx->state, ctx->buf,
				ctx->rate, ctx->rate);
			n = 0;
		}
	}
	ctx->count = n;
} /* end */

/* @func: sha3_finish - sha3 process the remaining bytes in the \
*                       buffer and end
* @param1: struct sha3_ctx * # sha3 struct context
* @return: void
*/
void FSYMBOL(sha3_finish)(struct sha3_ctx *ctx) {
	XSYMBOL(memset)(ctx->buf + ctx->count, 0, ctx->rate - ctx->count);
	ctx->buf[ctx->count] = 0x06;
	ctx->buf[ctx->rate - 1] = 0x80;

	_keccak_absorb(ctx->state, ctx->buf, ctx->rate, ctx->rate);
	_keccak_squeeze(ctx->state, ctx->digest, ctx->dsize, ctx->rate);
} /* end */

/* @func: sha3 - sha3 processing
* @param1: struct sha3_ctx * # sha3 struct context
* @param2: const uint8 *     # input buffer
* @param3: uint64            # input length
* @return: void
*/
void FSYMBOL(sha3)(struct sha3_ctx *ctx, const uint8 *s, uint64 len) {
	ctx->count = 0;
	FSYMBOL(sha3_process)(ctx, s, len);
	FSYMBOL(sha3_finish)(ctx);
} /* end */
