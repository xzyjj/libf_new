/* sha512.h - sha512 hash algorithm definitions */

#ifndef LIBF_AL_SHA512_H
#define LIBF_AL_SHA512_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: sha512 */
#undef SHA384_LEN
#define SHA384_LEN 48
#undef SHA512_LEN
#define SHA512_LEN 64
#undef SHA512_BLOCKSIZE
#define SHA512_BLOCKSIZE 128

#undef sha512_ctx
struct sha512_ctx {
	uint32 count;
	uint64L state[8];
	uint8 buf[SHA512_BLOCKSIZE];
};

#undef SHA512_NEW
#define	SHA512_NEW(x) struct sha512_ctx x
#undef SHA384_NEW
#define	SHA384_NEW(x) struct sha512_ctx x

#undef SHA512_STATE
#define SHA512_STATE(x, n) (((uint8 *)((x)->state))[n])
#undef SHA384_STATE
#define SHA384_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* sha512.c */
extern void FSYMBOL(sha512_init)(struct sha512_ctx *ctx);
extern void FSYMBOL(sha384_init)(struct sha512_ctx *ctx);
extern void FSYMBOL(sha512_process)(struct sha512_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(sha512_finish)(struct sha512_ctx *ctx, uint64L len);
extern void FSYMBOL(sha512)(struct sha512_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
