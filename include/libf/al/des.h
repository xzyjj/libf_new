/* des.h - data encryption standard definitions */

#ifndef LIBF_AL_DES_H
#define LIBF_AL_DES_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: des */
#undef DES_KEYLEN
#define DES_KEYLEN 8
#undef DES_BLOCKSIZE
#define DES_BLOCKSIZE 8
#undef DES_ENCRYPT
#define DES_ENCRYPT 0
#undef DES_DECRYPT
#define DES_DECRYPT 1

struct des_ctx {
	uint8 key_k[17][8];
	uint8 key_c[17][4];
	uint8 key_d[17][4];
	uint8 buf[DES_BLOCKSIZE];
	int32 count;
};

#undef DES_NEW
#define DES_NEW(x) struct des_ctx x
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* des.c */
void FSYMBOL(des_init)(struct des_ctx *ctx, const uint8 *key);
void FSYMBOL(des_crypto)(struct des_ctx *ctx, uint8 *buf, int32 is_decrypt);
int32 FSYMBOL(des_encrypt_process)(struct des_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len);
int32 FSYMBOL(des_encrypt_finish)(struct des_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg);
int32 FSYMBOL(des_decrypt_process)(struct des_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
