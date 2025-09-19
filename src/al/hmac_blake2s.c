/* hmac_blake2s.c - hash message authentication code (blake2s) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_blake2s.h>
#include <libf/al/blake2s.h>


/* @func: hmac_blake2s_init - hmac-blake2s key init
* @param1: struct hmac_blake2s_ctx * # hmac-blake2s struct context
* @param2: const uint8 *             # input key
* @param3: uint32                    # key length
* @return: void
*/
void FSYMBOL(hmac_blake2s_init)(struct hmac_blake2s_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	BLAKE2S_NEW(blake2s_ctx);
	FSYMBOL(blake2s_init)(&blake2s_ctx, BLAKE2S_256_LEN);

	if (key_len > BLAKE2S_BLOCKSIZE) {
		FSYMBOL(blake2s)(&blake2s_ctx, key, key_len);
		key = &(BLAKE2S_STATE(&blake2s_ctx, 0));
		key_len = BLAKE2S_256_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, BLAKE2S_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, BLAKE2S_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(blake2s_init)(&ctx->ipad_ctx, BLAKE2S_256_LEN);
	FSYMBOL(blake2s_init)(&ctx->opad_ctx, BLAKE2S_256_LEN);
	FSYMBOL(blake2s_process)(&ctx->ipad_ctx, ctx->ipad, BLAKE2S_BLOCKSIZE);
} /* end */

/* @func: hmac_blake2s_process - hmac-blake2s processing buffer
* @param1: struct hmac_blake2s_ctx * # hmac-blake2s struct context
* @param2: conat uint8 *             # input buffer
* @param3: uint64                    # input length
* @return: void
*/
void FSYMBOL(hmac_blake2s_process)(struct hmac_blake2s_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(blake2s_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_blake2s_finish - hmac-blake2s process the remaining bytes in the \
*                               buffer and end
* @param1: struct hmac_blake2s_ctx * # hmac-blake2s struct context
* @return: void
*/
void FSYMBOL(hmac_blake2s_finish)(struct hmac_blake2s_ctx *ctx) {
	FSYMBOL(blake2s_finish)(&ctx->ipad_ctx);

	FSYMBOL(blake2s_process)(&ctx->opad_ctx, ctx->opad, BLAKE2S_BLOCKSIZE);
	FSYMBOL(blake2s_process)(&ctx->opad_ctx,
		&(BLAKE2S_STATE(&ctx->ipad_ctx, 0)), BLAKE2S_256_LEN);
	FSYMBOL(blake2s_finish)(&ctx->opad_ctx);

	XSYMBOL(memcpy)(ctx->state, &(BLAKE2S_STATE(&ctx->opad_ctx, 0)),
		BLAKE2S_256_LEN);
} /* end */

/* @func: hmac_blake2s - hmac-blake2s processing
* @param1: struct hmac_blake2s_ctx * # hmac-blake2s struct context
* @param2: conat uint8 *             # input buffer
* @param3: uint64                    # input length
* @return: void
*/
void FSYMBOL(hmac_blake2s)(struct hmac_blake2s_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_blake2s_process)(ctx, s, len);
	FSYMBOL(hmac_blake2s_finish)(ctx);
} /* end */
