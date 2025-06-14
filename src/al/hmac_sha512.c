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
	FSYMBOL(sha512_init)(&sha512_ctx);

	if (key_len > SHA512_BLOCKSIZE) {
		FSYMBOL(sha512)(&sha512_ctx, key, key_len);
		key = &(SHA512_STATE(&sha512_ctx, 0));
		key_len = SHA512_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, SHA512_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, SHA512_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(sha512_init)(&ctx->ipad_ctx);
	FSYMBOL(sha512_init)(&ctx->opad_ctx);
	FSYMBOL(sha512_process)(&ctx->ipad_ctx, ctx->ipad, SHA512_BLOCKSIZE);
} /* end */

/* @func: hmac_sha512_process - hmac-sha512 processing buffer
* @param1: struct hmac_sha512_ctx * # hmac-sha512 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha512_process)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(sha512_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_sha512_finish - hmac-sha512 process the remaining bytes in the \
*                              buffer and end
* @param1: struct hmac_sha512_ctx * # hmac-sha512 struct context
* @param2: uint64L                  # total length
* @return: void
*/
void FSYMBOL(hmac_sha512_finish)(struct hmac_sha512_ctx *ctx, uint64L len) {
	FSYMBOL(sha512_finish)(&ctx->ipad_ctx, len + SHA512_BLOCKSIZE);

	FSYMBOL(sha512_process)(&ctx->opad_ctx, ctx->opad, SHA512_BLOCKSIZE);
	FSYMBOL(sha512_process)(&ctx->opad_ctx,
		&(SHA512_STATE(&ctx->ipad_ctx, 0)), SHA512_LEN);
	FSYMBOL(sha512_finish)(&ctx->opad_ctx, SHA512_BLOCKSIZE + SHA512_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA512_STATE(&ctx->opad_ctx, 0)),
		SHA512_LEN);
} /* end */

/* @func: hmac_sha512 - hmac-sha512 processing
* @param1: struct hmac_sha512_ctx * # hmac-sha512 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha512)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_sha512_process)(ctx, s, len);
	FSYMBOL(hmac_sha512_finish)(ctx, len);
} /* end */

/* @func: hmac_sha384_init - hmac-sha384 key init
* @param1: struct hmac_sha512_ctx * # hmac-sha384 struct context
* @param2: const uint8 *            # input key
* @param3: uint32                   # key length
* @return: void
*/
void FSYMBOL(hmac_sha384_init)(struct hmac_sha512_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	SHA384_NEW(sha384_ctx);
	FSYMBOL(sha384_init)(&sha384_ctx);

	if (key_len > SHA512_BLOCKSIZE) {
		FSYMBOL(sha512)(&sha384_ctx, key, key_len);
		key = &(SHA384_STATE(&sha384_ctx, 0));
		key_len = SHA384_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, SHA512_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, SHA512_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(sha384_init)(&ctx->ipad_ctx);
	FSYMBOL(sha384_init)(&ctx->opad_ctx);
	FSYMBOL(sha512_process)(&ctx->ipad_ctx, ctx->ipad, SHA512_BLOCKSIZE);
} /* end */

/* @func: hmac_sha384_process - hmac-sha384 processing buffer
* @param1: struct hmac_sha512_ctx * # hmac-sha384 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha384_process)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(sha512_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_sha384_finish - hmac-sha384 process the remaining bytes in the \
*                              buffer and end
* @param1: struct hmac_sha512_ctx * # hmac-sha384 struct context
* @param2: uint64L                  # total length
* @return: void
*/
void FSYMBOL(hmac_sha384_finish)(struct hmac_sha512_ctx *ctx, uint64L len) {
	FSYMBOL(sha512_finish)(&ctx->ipad_ctx, len + SHA512_BLOCKSIZE);

	FSYMBOL(sha512_process)(&ctx->opad_ctx, ctx->opad, SHA512_BLOCKSIZE);
	FSYMBOL(sha512_process)(&ctx->opad_ctx,
		&(SHA384_STATE(&ctx->ipad_ctx, 0)), SHA384_LEN);
	FSYMBOL(sha512_finish)(&ctx->opad_ctx, SHA512_BLOCKSIZE + SHA384_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA384_STATE(&ctx->opad_ctx, 0)),
		SHA384_LEN);
} /* end */

/* @func: hmac_sha384 - hmac-sha384 processing
* @param1: struct hmac_sha512_ctx * # hmac-sha384 struct context
* @param2: conat uint8 *            # input buffer
* @param3: uint64                   # input length
* @return: void
*/
void FSYMBOL(hmac_sha384)(struct hmac_sha512_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_sha512_process)(ctx, s, len);
	FSYMBOL(hmac_sha512_finish)(ctx, len);
} /* end */
