/* hmac_sha3.h - hash message authentication code (sha3) definitions */

#ifndef LIBF_AL_HMAC_SHA3_H
#define LIBF_AL_HMAC_SHA3_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha3.h>


/* @def: hmac_sha3 */
#undef hmac_sha3_ctx
struct hmac_sha3_ctx {
	uint8 ipad[SHA3_STATE_SIZE];
	uint8 opad[SHA3_STATE_SIZE];
	struct sha3_ctx ipad_ctx;
	struct sha3_ctx opad_ctx;
	uint8 state[SHA3_STATE_SIZE];
};

#undef HMAC_SHA3_NEW
#define HMAC_SHA3_NEW(x) struct hmac_sha3_ctx x
#undef HMAC_SHA3_STATE
#define HMAC_SHA3_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* hmac_sha3.c */
extern int32 FSYMBOL(hmac_sha3_init)(struct hmac_sha3_ctx *ctx, const uint8 *key,
		uint32 key_len, int32 type, uint32 dsize);
extern void FSYMBOL(hmac_sha3_process)(struct hmac_sha3_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_sha3_finish)(struct hmac_sha3_ctx *ctx);
extern void FSYMBOL(hmac_sha3)(struct hmac_sha3_ctx *ctx, const uint8 *s,
		uint64 len);

#ifdef __cplusplus
}
#endif


#endif
