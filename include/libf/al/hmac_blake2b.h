/* hmac_blake2b.h - hash message authentication code (blake2b) definitions */

#ifndef LIBF_AL_HMAC_BLAKE2B_H
#define LIBF_AL_HMAC_BLAKE2B_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/blake2b.h>


/* @def: hmac_blake2b */
#undef hmac_blake2b_ctx
struct hmac_blake2b_ctx {
	uint8 ipad[BLAKE2B_BLOCKSIZE];
	uint8 opad[BLAKE2B_BLOCKSIZE];
	struct blake2b_ctx ipad_ctx;
	struct blake2b_ctx opad_ctx;
	uint8 state[BLAKE2B_512_LEN];
};

#undef HMAC_BLAKE2B_NEW
#define HMAC_BLAKE2B_NEW(x) struct hmac_blake2b_ctx x
#undef HMAC_BLAKE2B_STATE
#define HMAC_BLAKE2B_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* hmac_blake2b.c */
extern void FSYMBOL(hmac_blake2b_init)(struct hmac_blake2b_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_blake2b_process)(struct hmac_blake2b_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_blake2b_finish)(struct hmac_blake2b_ctx *ctx);
extern void FSYMBOL(hmac_blake2b)(struct hmac_blake2b_ctx *ctx, const uint8 *s,
		uint64 len);

#ifdef __cplusplus
}
#endif


#endif
