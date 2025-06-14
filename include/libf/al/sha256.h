/* sha256.h - sha256 hash algorithm definitions */

#ifndef LIBF_AL_SHA256_H
#define LIBF_AL_SHA256_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: sha256 */
#undef SHA224_LEN
#define SHA224_LEN 28
#undef SHA256_LEN
#define SHA256_LEN 32
#undef SHA256_BLOCKSIZE
#define SHA256_BLOCKSIZE 64

#undef sha256_ctx
struct sha256_ctx {
	uint32 count;
	uint32 state[8];
	uint8 buf[SHA256_BLOCKSIZE];
};

#undef SHA256_NEW
#define	SHA256_NEW(x) struct sha256_ctx x
#undef SHA224_NEW
#define	SHA224_NEW(x) struct sha256_ctx x

#undef SHA256_STATE
#define SHA256_STATE(x, n) (((uint8 *)((x)->state))[n])
#undef SHA224_STATE
#define SHA224_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* sha256.c */
extern void FSYMBOL(sha256_init)(struct sha256_ctx *ctx);
extern void FSYMBOL(sha224_init)(struct sha256_ctx *ctx);
extern void FSYMBOL(sha256_process)(struct sha256_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(sha256_finish)(struct sha256_ctx *ctx, uint64L len);
extern void FSYMBOL(sha256)(struct sha256_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
