/* hmac_sha256.h - hash message authentication code (sha256) definitions */

#ifndef LIBF_AL_HMAC_SHA256_H
#define LIBF_AL_HMAC_SHA256_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha256.h>


/* @def: hmac_sha256 */
#undef hmac_sha256_ctx
struct hmac_sha256_ctx {
	uint8 ipad[SHA256_BLOCKSIZE];
	uint8 opad[SHA256_BLOCKSIZE];
	struct sha256_ctx ipad_ctx;
	struct sha256_ctx opad_ctx;
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
extern void FSYMBOL(hmac_sha256_process)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_sha256_finish)(struct hmac_sha256_ctx *ctx, uint64L len);
extern void FSYMBOL(hmac_sha256)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_sha224_init)(struct hmac_sha256_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_sha224_process)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_sha224_finish)(struct hmac_sha256_ctx *ctx, uint64L len);
extern void FSYMBOL(hmac_sha224)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len);

#ifdef __cplusplus
}
#endif


#endif
