/* hmac_sha1.c - hash message authentication code (sha1) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha1.h>
#include <libf/al/sha1.h>


/* @func: hmac_sha1_init - hmac-sha1 key init
* @param1: struct hmac_sha1_ctx * # hmac-sha1 struct context
* @param2: const uint8 *          # input key
* @param3: uint32                 # key length
* @return: void
*/
void FSYMBOL(hmac_sha1_init)(struct hmac_sha1_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	SHA1_NEW(sha1_ctx);
	FSYMBOL(sha1_init)(&sha1_ctx);

	if (key_len > SHA1_BLOCKSIZE) {
		FSYMBOL(sha1)(&sha1_ctx, key, key_len);
		key = &(SHA1_STATE(&sha1_ctx, 0));
		key_len = SHA1_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, SHA1_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, SHA1_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(sha1_init)(&ctx->ipad_ctx);
	FSYMBOL(sha1_init)(&ctx->opad_ctx);
	FSYMBOL(sha1_process)(&ctx->ipad_ctx, ctx->ipad, SHA1_BLOCKSIZE);
} /* end */

/* @func: hmac_sha1_process - hmac-sha1 processing buffer
* @param1: struct hmac_sha1_ctx * # hmac-sha1 struct context
* @param2: conat uint8 *          # input buffer
* @param3: uint64                 # input length
* @return: void
*/
void FSYMBOL(hmac_sha1_process)(struct hmac_sha1_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(sha1_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_sha1_finish - hmac-sha1 process the remaining bytes in the \
*                            buffer and end
* @param1: struct hmac_sha1_ctx * # hmac-sha1 struct context
* @param2: uint64L                # total length
* @return: void
*/
void FSYMBOL(hmac_sha1_finish)(struct hmac_sha1_ctx *ctx, uint64L len) {
	FSYMBOL(sha1_finish)(&ctx->ipad_ctx, len + SHA1_BLOCKSIZE);

	FSYMBOL(sha1_process)(&ctx->opad_ctx, ctx->opad, SHA1_BLOCKSIZE);
	FSYMBOL(sha1_process)(&ctx->opad_ctx, &(SHA1_STATE(&ctx->ipad_ctx, 0)),
		SHA1_LEN);
	FSYMBOL(sha1_finish)(&ctx->opad_ctx, SHA1_BLOCKSIZE + SHA1_LEN);

	XSYMBOL(memcpy)(ctx->state, &(SHA1_STATE(&ctx->opad_ctx, 0)), SHA1_LEN);
} /* end */

/* @func: hmac_sha1 - hmac-sha1 processing
* @param1: struct hmac_sha1_ctx * # hmac-sha1 struct context
* @param2: conat uint8 *          # input buffer
* @param3: uint64                 # input length
* @return: void
*/
void FSYMBOL(hmac_sha1)(struct hmac_sha1_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_sha1_process)(ctx, s, len);
	FSYMBOL(hmac_sha1_finish)(ctx, len);
} /* end */
