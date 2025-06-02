/* sha3.h - secure hash-3 algorithm definitions */

#ifndef LIBF_AL_SHA3_H
#define LIBF_AL_SHA3_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: sha3 */
#undef SHA3_224_TYPE
#define SHA3_224_TYPE 1
#undef SHA3_224_LEN
#define SHA3_224_LEN 28
/* (1600 - (2 * 224)) / 8 */
#undef SHA3_224_RATE
#define SHA3_224_RATE 144

#undef SHA3_256_TYPE
#define SHA3_256_TYPE 2
#undef SHA3_256_LEN
#define SHA3_256_LEN 32
/* (1600 - (2 * 256)) / 8 */
#undef SHA3_256_RATE
#define SHA3_256_RATE 136

#undef SHA3_384_TYPE
#define SHA3_384_TYPE 3
#undef SHA3_384_LEN
#define SHA3_384_LEN 48
/* (1600 - (2 * 384)) / 8 */
#undef SHA3_384_RATE
#define SHA3_384_RATE 104

#undef SHA3_512_TYPE
#define SHA3_512_TYPE 4
#undef SHA3_512_LEN
#define SHA3_512_LEN 64
/* (1600 - (2 * 512)) / 8 */
#undef SHA3_512_RATE
#define SHA3_512_RATE 72

#undef SHA3_STATE_SIZE
#define SHA3_STATE_SIZE (5 * 5 * 8)
#undef SHA3_KECCAK_ROUNDS
#define SHA3_KECCAK_ROUNDS 24

#undef sha3_ctx
struct sha3_ctx {
	uint64L state[5][5];
	uint8 buf[SHA3_STATE_SIZE];
	uint8 digest[SHA3_512_LEN];
	uint32 rate;
	uint32 dsize;
	uint32 count;
};

#undef SHA3_NEW
#define SHA3_NEW(x) struct sha3_ctx x

#undef SHA3_STATE
#define SHA3_STATE(x, n) ((x)->digest[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* sha3.c */
extern int32 FSYMBOL(sha3_init)(struct sha3_ctx *ctx, int32 type);
extern void FSYMBOL(sha3_process)(struct sha3_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(sha3_finish)(struct sha3_ctx *ctx);
extern void FSYMBOL(sha3)(struct sha3_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
