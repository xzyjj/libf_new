/* hmac_sha512.h - hash message authentication code (sha512) definitions */

#ifndef LIBF_AL_HMAC_SHA512_H
#define LIBF_AL_HMAC_SHA512_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha512.h>


/* @def: hmac_sha512 */
#undef hmac_sha512_ctx
struct hmac_sha512_ctx {
	uint8 ipad[SHA512_BLOCKSIZE];
	uint8 opad[SHA512_BLOCKSIZE];
	struct sha512_ctx ipad_ctx;
	struct sha512_ctx opad_ctx;
	uint8 state[SHA512_LEN];
};

#undef HMAC_SHA512_NEW
#define HMAC_SHA512_NEW(x) struct hmac_sha512_ctx x
#undef HMAC_SHA512_STATE
#define HMAC_SHA512_STATE(x, n) (((uint8 *)((x)->state))[n])

#undef HMAC_SHA384_NEW
#define HMAC_SHA384_NEW(x) struct hmac_sha512_ctx x
#undef HMAC_SHA384_STATE
#define HMAC_SHA384_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* hmac_sha512.c */
extern void FSYMBOL(hmac_sha512_init)(struct hmac_sha512_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_sha512_process)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_sha512_finish)(struct hmac_sha512_ctx *ctx, uint64L len);
extern void FSYMBOL(hmac_sha512)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_sha384_init)(struct hmac_sha512_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_sha384_process)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_sha384_finish)(struct hmac_sha512_ctx *ctx, uint64L len);
extern void FSYMBOL(hmac_sha384)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len);

#ifdef __cplusplus
}
#endif


#endif
