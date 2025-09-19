/* hmac_blake2s.h - hash message authentication code (blake2s) definitions */

#ifndef LIBF_AL_HMAC_BLAKE2S_H
#define LIBF_AL_HMAC_BLAKE2S_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/blake2s.h>


/* @def: hmac_blake2s */
#undef hmac_blake2s_ctx
struct hmac_blake2s_ctx {
	uint8 ipad[BLAKE2S_BLOCKSIZE];
	uint8 opad[BLAKE2S_BLOCKSIZE];
	struct blake2s_ctx ipad_ctx;
	struct blake2s_ctx opad_ctx;
	uint8 state[BLAKE2S_256_LEN];
};

#undef HMAC_BLAKE2S_NEW
#define HMAC_BLAKE2S_NEW(x) struct hmac_blake2s_ctx x
#undef HMAC_BLAKE2S_STATE
#define HMAC_BLAKE2S_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* hmac_blake2s.c */
extern void FSYMBOL(hmac_blake2s_init)(struct hmac_blake2s_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_blake2s_process)(struct hmac_blake2s_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_blake2s_finish)(struct hmac_blake2s_ctx *ctx);
extern void FSYMBOL(hmac_blake2s)(struct hmac_blake2s_ctx *ctx, const uint8 *s,
		uint64 len);

#ifdef __cplusplus
}
#endif


#endif
