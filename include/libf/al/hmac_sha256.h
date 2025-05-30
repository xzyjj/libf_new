/* hmac_sha256.h - hash message authentication code (sha256) definitions */

#ifndef LIBF_AL_HMAC_SHA256_H
#define LIBF_AL_HMAC_SHA256_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha256.h>


/* @def: hmac_256 */
#undef HAMC_SHA256_BLOCKSIZE
#define HAMC_SHA256_BLOCKSIZE 64
#undef HAMC_SHA256_LEN
#define HAMC_SHA256_LEN SHA256_LEN

#undef HAMC_SHA224_BLOCKSIZE
#define HAMC_SHA224_BLOCKSIZE 64
#undef HAMC_SHA224_LEN
#define HAMC_SHA224_LEN SHA224_LEN

#undef hmac_sha256_ctx
struct hmac_sha256_ctx {
	uint8 ipad[HAMC_SHA256_BLOCKSIZE];
	uint8 opad[HAMC_SHA256_BLOCKSIZE];
	uint8 state[SHA256_LEN];
};

#undef HMAC_SHA256_NEW
#define HMAC_SHA256_NEW(x) struct hmac_sha256_ctx x
#undef HMAC_SHA256_STATE
#define HMAC_SHA256_STATE(x, n) (((uint8 *)((x)->state))[n])

#undef HMAC_SHA224_NEW
#define HMAC_SHA224_NEW(x) struct hmac_sha256_ctx x
#undef HMAC_SHA224_STATE
#define HMAC_SHA224_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* hmac_sha256.c */
extern void FSYMBOL(hmac_sha256_init)(struct hmac_sha256_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_sha256)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint32 len);
extern void FSYMBOL(hmac_sha224_init)(struct hmac_sha256_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_sha224)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint32 len);

#ifdef __cplusplus
}
#endif


#endif
