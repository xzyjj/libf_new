/* hmac_sha512.c - hash message authentication code (sha512) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha512.h>
#include <libf/al/sha512.h>


/* @func: hmac_sha512_init - hmac-sha512 key init
* @param1: struct hmac_sha512_ctx * # hmac-sha512 struct context
* @param2: const uint8 *            # input key
* @param3: uint32                   # key length
* @return: void
*/
void FSYMBOL(hmac_sha512_init)(struct hmac_sha512_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	SHA512_NEW(sha512_ctx);
	if (key_len > HAMC_SHA512_BLOCKSIZE) {
		FSYMBOL(sha512)(&sha512_ctx, key, key_len);
		key = &(SHA512_STATE(&sha512_ctx, 0));
		key_len = SHA512_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, HAMC_SHA512_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, HAMC_SHA512_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}
} /* end */

/* @func: hmac_sha512 - hmac-sha512 digest function
* @param1: struct hmac_sha512_ctx * # hmac-sha512 struct context
* @param2: const uint8 *            # input message
* @param3: uint32                   # message length
* @return: void
*/
void FSYMBOL(hmac_sha512)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint32 len) {
	SHA512_NEW(sha512_ipad_ctx);
	SHA512_NEW(sha512_opad_ctx);

	FSYMBOL(sha512_process)(&sha512_ipad_ctx, ctx->ipad,
		HAMC_SHA512_BLOCKSIZE);
	FSYMBOL(sha512_process)(&sha512_ipad_ctx, s, len);
	FSYMBOL(sha512_finish)(&sha512_ipad_ctx, len
		+ HAMC_SHA512_BLOCKSIZE);

	FSYMBOL(sha512_process)(&sha512_opad_ctx, ctx->opad,
		HAMC_SHA512_BLOCKSIZE);
	FSYMBOL(sha512_process)(&sha512_opad_ctx,
		&(SHA512_STATE(&sha512_ipad_ctx, 0)), SHA512_LEN);
	FSYMBOL(sha512_finish)(&sha512_opad_ctx, HAMC_SHA512_BLOCKSIZE
		+ SHA512_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA512_STATE(&sha512_opad_ctx, 0)),
		SHA512_LEN);
} /* end */

/* @func: hmac_sha384_init - hmac-sha384 key init
* @param1: struct hmac_sha512_ctx * # hmac-sha512 struct context
* @param2: const uint8 *            # input key
* @param3: uint32                   # key length
* @return: void
*/
void FSYMBOL(hmac_sha384_init)(struct hmac_sha512_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	SHA384_NEW(sha384_ctx);
	if (key_len > HAMC_SHA384_BLOCKSIZE) {
		FSYMBOL(sha512)(&sha384_ctx, key, key_len);
		key = &(SHA384_STATE(&sha384_ctx, 0));
		key_len = SHA384_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, HAMC_SHA384_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, HAMC_SHA384_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}
} /* end */

/* @func: hmac_sha384 - hmac-sha384 digest function
* @param1: struct hmac_sha512_ctx * # hmac-sha512 struct context
* @param2: const uint8 *            # input message
* @param3: uint32                   # message length
* @return: void
*/
void FSYMBOL(hmac_sha384)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint32 len) {
	SHA384_NEW(sha384_ipad_ctx);
	SHA384_NEW(sha384_opad_ctx);

	FSYMBOL(sha512_process)(&sha384_ipad_ctx, ctx->ipad,
		HAMC_SHA384_BLOCKSIZE);
	FSYMBOL(sha512_process)(&sha384_ipad_ctx, s, len);
	FSYMBOL(sha512_finish)(&sha384_ipad_ctx, len
		+ HAMC_SHA384_BLOCKSIZE);

	FSYMBOL(sha512_process)(&sha384_opad_ctx, ctx->opad,
		HAMC_SHA384_BLOCKSIZE);
	FSYMBOL(sha512_process)(&sha384_opad_ctx,
		&(SHA384_STATE(&sha384_ipad_ctx, 0)), SHA384_LEN);
	FSYMBOL(sha512_finish)(&sha384_opad_ctx, HAMC_SHA384_BLOCKSIZE
		+ SHA384_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA384_STATE(&sha384_opad_ctx, 0)),
		SHA384_LEN);
} /* end */
