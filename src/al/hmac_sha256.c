/* hmac_sha256.c - hash message authentication code (sha256) implementations */

#include <libf/config.h>
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
	FSYMBOL(sha256_init)(&sha256_ctx);

	if (key_len > SHA256_BLOCKSIZE) {
		FSYMBOL(sha256)(&sha256_ctx, key, key_len);
		key = &(SHA256_STATE(&sha256_ctx, 0));
		key_len = SHA256_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, SHA256_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, SHA256_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(sha256_init)(&ctx->ipad_ctx);
	FSYMBOL(sha256_init)(&ctx->opad_ctx);
	FSYMBOL(sha256_process)(&ctx->ipad_ctx, ctx->ipad, SHA256_BLOCKSIZE);
} /* end */

/* @func: hmac_sha256_process - hmac-sha256 processing buffer
* @param1: struct hmac_sha256_ctx * # hmac-sha256 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha256_process)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(sha256_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_sha256_finish - hmac-sha256 process the remaining bytes in the \
*                              buffer and end
* @param1: struct hmac_sha256_ctx * # hmac-sha256 struct context
* @param2: uint64L                  # total length
* @return: void
*/
void FSYMBOL(hmac_sha256_finish)(struct hmac_sha256_ctx *ctx, uint64L len) {
	FSYMBOL(sha256_finish)(&ctx->ipad_ctx, len + SHA256_BLOCKSIZE);

	FSYMBOL(sha256_process)(&ctx->opad_ctx, ctx->opad, SHA256_BLOCKSIZE);
	FSYMBOL(sha256_process)(&ctx->opad_ctx,
		&(SHA256_STATE(&ctx->ipad_ctx, 0)), SHA256_LEN);
	FSYMBOL(sha256_finish)(&ctx->opad_ctx, SHA256_BLOCKSIZE + SHA256_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA256_STATE(&ctx->opad_ctx, 0)),
		SHA256_LEN);
} /* end */

/* @func: hmac_sha256 - hmac-sha256 processing
* @param1: struct hmac_sha256_ctx * # hmac-sha256 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha256)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_sha256_process)(ctx, s, len);
	FSYMBOL(hmac_sha256_finish)(ctx, len);
} /* end */

/* @func: hmac_sha224_init - hmac-sha224 key init
* @param1: struct hmac_sha256_ctx * # hmac-sha224 struct context
* @param2: const uint8 *            # input key
* @param3: uint32                   # key length
* @return: void
*/
void FSYMBOL(hmac_sha224_init)(struct hmac_sha256_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	SHA224_NEW(sha224_ctx);
	FSYMBOL(sha224_init)(&sha224_ctx);

	if (key_len > SHA256_BLOCKSIZE) {
		FSYMBOL(sha256)(&sha224_ctx, key, key_len);
		key = &(SHA224_STATE(&sha224_ctx, 0));
		key_len = SHA224_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, SHA256_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, SHA256_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(sha224_init)(&ctx->ipad_ctx);
	FSYMBOL(sha224_init)(&ctx->opad_ctx);
	FSYMBOL(sha256_process)(&ctx->ipad_ctx, ctx->ipad, SHA256_BLOCKSIZE);
} /* end */

/* @func: hmac_sha224_process - hmac-sha224 processing buffer
* @param1: struct hmac_sha256_ctx * # hmac-sha224 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha224_process)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(sha256_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_sha224_finish - hmac-sha224 process the remaining bytes in the \
*                              buffer and end
* @param1: struct hmac_sha256_ctx * # hmac-sha224 struct context
* @param2: uint64L                  # total length
* @return: void
*/
void FSYMBOL(hmac_sha224_finish)(struct hmac_sha256_ctx *ctx, uint64L len) {
	FSYMBOL(sha256_finish)(&ctx->ipad_ctx, len + SHA256_BLOCKSIZE);

	FSYMBOL(sha256_process)(&ctx->opad_ctx, ctx->opad, SHA256_BLOCKSIZE);
	FSYMBOL(sha256_process)(&ctx->opad_ctx,
		&(SHA224_STATE(&ctx->ipad_ctx, 0)), SHA224_LEN);
	FSYMBOL(sha256_finish)(&ctx->opad_ctx, SHA256_BLOCKSIZE + SHA224_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA224_STATE(&ctx->opad_ctx, 0)),
		SHA224_LEN);
} /* end */

/* @func: hmac_sha224 - hmac-sha224 processing
* @param1: struct hmac_sha256_ctx * # hmac-sha224 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha224)(struct hmac_sha256_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_sha256_process)(ctx, s, len);
	FSYMBOL(hmac_sha256_finish)(ctx, len);
} /* end */
