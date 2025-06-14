/* sha1.h - sha1 hash algorithm definitions */

#ifndef LIBF_AL_SHA1_H
#define LIBF_AL_SHA1_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: sha1 */
#undef SHA1_LEN
#define SHA1_LEN 20
#undef SHA1_BLOCKSIZE
#define SHA1_BLOCKSIZE 64

#undef sha1_ctx
struct sha1_ctx {
	uint32 count;
	uint32 state[5];
	uint8 buf[SHA1_BLOCKSIZE];
};

#undef SHA1_NEW
#define	SHA1_NEW(x) struct sha1_ctx x

#undef SHA1_STATE
#define SHA1_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* sha1.c */
extern void FSYMBOL(sha1_init)(struct sha1_ctx *ctx);
extern void FSYMBOL(sha1_process)(struct sha1_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(sha1_finish)(struct sha1_ctx *ctx, uint64L len);
extern void FSYMBOL(sha1)(struct sha1_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
