/* aes.h - advanced encryption standard definitions */

#ifndef LIBF_AL_AES_H
#define LIBF_AL_AES_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: aes
* aes128 */
#undef AES_128_TYPE
#define AES_128_TYPE 0
#undef AES_128_KEYLEN
#define AES_128_KEYLEN 16
#undef AES_128_ROUNDS
#define AES_128_ROUNDS 10

/* aes192 */
#undef AES_192_TYPE
#define AES_192_TYPE 1
#undef AES_192_KEYLEN
#define AES_192_KEYLEN 24
#undef AES_192_ROUNDS
#define AES_192_ROUNDS 12

/* aes256 */
#undef AES_256_TYPE
#define AES_256_TYPE 2
#undef AES_256_KEYLEN
#define AES_256_KEYLEN 32
#undef AES_256_ROUNDS
#define AES_256_ROUNDS 14

#undef AES_BLOCKSIZE
#define AES_BLOCKSIZE 16
#undef AES_KEYEXPLEN
#define AES_KEYEXPLEN (AES_BLOCKSIZE * (AES_256_ROUNDS + 1))

struct aes_ctx {
	uint8 keyexp[AES_KEYEXPLEN];
	uint8 buf[AES_BLOCKSIZE];
	int32 count;
	int32 keylen;
	int32 rounds;
	int32 type;
};

#undef AES_NEW
#define AES_NEW(x) struct aes_ctx x
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* aes.c */
extern int32 FSYMBOL(aes_init)(struct aes_ctx *ctx, const uint8 *key, int32 type);
extern void FSYMBOL(aes_encrypt)(struct aes_ctx *ctx, uint8 *state);
extern void FSYMBOL(aes_decrypt)(struct aes_ctx *ctx, uint8 *state);
extern int32 FSYMBOL(aes_encrypt_process)(struct aes_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len);
extern int32 FSYMBOL(aes_encrypt_finish)(struct aes_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg);
extern int32 FSYMBOL(aes_decrypt_process)(struct aes_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
