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

#undef aes_ctx
struct aes_ctx {
	uint8 keyexp[AES_KEYEXPLEN];
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
extern int32 FSYMBOL(aes_init)(struct aes_ctx *ctx, const uint8 *key,
		int32 type);
extern void FSYMBOL(aes_encrypt)(struct aes_ctx *ctx, uint8 *state);
extern void FSYMBOL(aes_decrypt)(struct aes_ctx *ctx, uint8 *state);
extern void FSYMBOL(aes_encrypt_cbc)(struct aes_ctx *ctx, uint8 *iv,
		uint8 *state);
extern void FSYMBOL(aes_decrypt_cbc)(struct aes_ctx *ctx, uint8 *iv,
		uint8 *state);
extern void FSYMBOL(aes_encrypt_cfb)(struct aes_ctx *ctx, uint8 *iv,
		uint8 *state);
extern void FSYMBOL(aes_decrypt_cfb)(struct aes_ctx *ctx, uint8 *iv,
		uint8 *state);
extern void FSYMBOL(aes_crypto_ofb)(struct aes_ctx *ctx, uint8 *iv,
		uint8 *buf, uint64 len);
extern void FSYMBOL(aes_crypto_ctr)(struct aes_ctx *ctx, uint8 *iv,
		uint8 *buf, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
