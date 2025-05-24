/* hmac_sha256.c - hash message authentication code (sha256) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha256.h>
#include <libf/al/sha256.h>


/* @func: hmac_sha256_init - hmac-sha256 key init
* @param1: struct hmac_sha256_ctx * # hmac-sha256 struct context
* @param2: const uint8 *            # input key
* @param3: uint32                   # key length
* @return: void
*/
void FSYMBOL(hmac_sha256_init)(struct hmac_sha256_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	SHA256_NEW(sha256_ctx);
	if (key_len > HAMC_SHA256_BLOCKSIZE) {
		FSYMBOL(sha256)(&sha256_ctx, key, key_len);
		key = &(SHA256_STATE(&sha256_ctx, 0));
		key_len = SHA256_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, HAMC_SHA256_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, HAMC_SHA256_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}
} /* end */

/* @func: hmac_sha256 - hmac-sha256 digest function
* @param1: struct hmac_sha256_ctx * # hmac-sha256 struct context
* @param2: const uint8 *            # input message
* @param3: uint32                   # message length
* @return: void
*/
void FSYMBOL(hmac_sha256)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint32 len) {
	SHA256_NEW(sha256_ipad_ctx);
	SHA256_NEW(sha256_opad_ctx);

	FSYMBOL(sha256_process)(&sha256_ipad_ctx, ctx->ipad,
		HAMC_SHA256_BLOCKSIZE);
	FSYMBOL(sha256_process)(&sha256_ipad_ctx, s, len);
	FSYMBOL(sha256_finish)(&sha256_ipad_ctx, len
		+ HAMC_SHA256_BLOCKSIZE);

	FSYMBOL(sha256_process)(&sha256_opad_ctx, ctx->opad,
		HAMC_SHA256_BLOCKSIZE);
	FSYMBOL(sha256_process)(&sha256_opad_ctx,
		&(SHA256_STATE(&sha256_ipad_ctx, 0)), SHA256_LEN);
	FSYMBOL(sha256_finish)(&sha256_opad_ctx, HAMC_SHA256_BLOCKSIZE
		+ SHA256_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA256_STATE(&sha256_opad_ctx, 0)),
		SHA256_LEN);
} /* end */

/* @func: hmac_sha224_init - hmac-sha224 key init
* @param1: struct hmac_sha256_ctx * # hmac-sha256 struct context
* @param2: const uint8 *            # input key
* @param3: uint32                   # key length
* @return: void
*/
void FSYMBOL(hmac_sha224_init)(struct hmac_sha256_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	SHA224_NEW(sha224_ctx);
	if (key_len > HAMC_SHA224_BLOCKSIZE) {
		FSYMBOL(sha256)(&sha224_ctx, key, key_len);
		key = &(SHA224_STATE(&sha224_ctx, 0));
		key_len = SHA224_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, HAMC_SHA224_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, HAMC_SHA224_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}
} /* end */

/* @func: hmac_sha224 - hmac-sha224 digest function
* @param1: struct hmac_sha256_ctx * # hmac-sha256 struct context
* @param2: const uint8 *            # input message
* @param3: uint32                   # message length
* @return: void
*/
void FSYMBOL(hmac_sha224)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint32 len) {
	SHA224_NEW(sha224_ipad_ctx);
	SHA224_NEW(sha224_opad_ctx);

	FSYMBOL(sha256_process)(&sha224_ipad_ctx, ctx->ipad,
		HAMC_SHA224_BLOCKSIZE);
	FSYMBOL(sha256_process)(&sha224_ipad_ctx, s, len);
	FSYMBOL(sha256_finish)(&sha224_ipad_ctx, len
		+ HAMC_SHA224_BLOCKSIZE);

	FSYMBOL(sha256_process)(&sha224_opad_ctx, ctx->opad,
		HAMC_SHA224_BLOCKSIZE);
	FSYMBOL(sha256_process)(&sha224_opad_ctx,
		&(SHA224_STATE(&sha224_ipad_ctx, 0)), SHA224_LEN);
	FSYMBOL(sha256_finish)(&sha224_opad_ctx, HAMC_SHA224_BLOCKSIZE
		+ SHA224_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA224_STATE(&sha224_opad_ctx, 0)),
		SHA224_LEN);
} /* end */
