/* hmac_md5.h - hash message authentication code (md5) definitions */

#ifndef LIBF_AL_HMAC_MD5_H
#define LIBF_AL_HMAC_MD5_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/md5.h>


/* @def: hmac_md5 */
#undef HAMC_MD5_BLOCKSIZE
#define HAMC_MD5_BLOCKSIZE 64
#undef HAMC_MD5_LEN
#define HAMC_MD5_LEN MD5_LEN

#undef hmac_md5_ctx
struct hmac_md5_ctx {
	uint8 ipad[HAMC_MD5_BLOCKSIZE];
	uint8 opad[HAMC_MD5_BLOCKSIZE];
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
extern void FSYMBOL(hmac_md5)(struct hmac_md5_ctx *ctx, const uint8 *s,
		uint32 len);

#ifdef __cplusplus
}
#endif


#endif
