/* hmac_sha1.h - hash message authentication code (sha1) definitions */

#ifndef LIBF_AL_HMAC_SHA1_H
#define LIBF_AL_HMAC_SHA1_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha1.h>


/* @def: hmac_sha1 */
#undef HAMC_SHA1_BLOCKSIZE
#define HAMC_SHA1_BLOCKSIZE 64
#undef HAMC_SHA1_LEN
#define HAMC_SHA1_LEN SHA1_LEN

struct hmac_sha1_ctx {
	uint8 ipad[HAMC_SHA1_BLOCKSIZE];
	uint8 opad[HAMC_SHA1_BLOCKSIZE];
	uint8 state[SHA1_LEN];
};

#undef HMAC_SHA1_NEW
#define HMAC_SHA1_NEW(x) struct hmac_sha1_ctx x
#undef HMAC_SHA1_STATE
#define HMAC_SHA1_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* hmac_sha1.c */
extern void FSYMBOL(hmac_sha1_init)(struct hmac_sha1_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_sha1)(struct hmac_sha1_ctx *ctx, const uint8 *s,
		uint32 len);

#ifdef __cplusplus
}
#endif


#endif
