/* hmac_md5.h - hash message authentication code (md5) definitions */

#ifndef LIBF_AL_HMAC_MD5_H
#define LIBF_AL_HMAC_MD5_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/md5.h>


/* @def: hmac_md5 */
#undef hmac_md5_ctx
struct hmac_md5_ctx {
	uint8 ipad[MD5_BLOCKSIZE];
	uint8 opad[MD5_BLOCKSIZE];
	struct md5_ctx ipad_ctx;
	struct md5_ctx opad_ctx;
	uint8 state[MD5_LEN];
};

#undef HMAC_MD5_NEW
#define HMAC_MD5_NEW(x) struct hmac_md5_ctx x
#undef HMAC_MD5_STATE
#define HMAC_MD5_STATE(x, n) (((uint8 *)((x)->state))[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* hmac_md5.c */
extern void FSYMBOL(hmac_md5_init)(struct hmac_md5_ctx *ctx, const uint8 *key,
		uint32 key_len);
extern void FSYMBOL(hmac_md5_process)(struct hmac_md5_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(hmac_md5_finish)(struct hmac_md5_ctx *ctx, uint64L len);
extern void FSYMBOL(hmac_md5)(struct hmac_md5_ctx *ctx, const uint8 *s,
		uint64 len);

#ifdef __cplusplus
}
#endif


#endif
