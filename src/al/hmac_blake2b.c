/* hmac_blake2b.c - hash message authentication code (blake2b) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_blake2b.h>
#include <libf/al/blake2b.h>


/* @func: hmac_blake2b_init - hmac-blake2b key init
* @param1: struct hmac_blake2b_ctx * # hmac-blake2b struct context
* @param2: const uint8 *             # input key
* @param3: uint32                    # key length
* @return: void
*/
void FSYMBOL(hmac_blake2b_init)(struct hmac_blake2b_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	BLAKE2B_NEW(blake2b_ctx);
	FSYMBOL(blake2b_init)(&blake2b_ctx, BLAKE2B_512_LEN);

	if (key_len > BLAKE2B_BLOCKSIZE) {
		FSYMBOL(blake2b)(&blake2b_ctx, key, key_len);
		key = &(BLAKE2B_STATE(&blake2b_ctx, 0));
		key_len = BLAKE2B_512_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, BLAKE2B_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, BLAKE2B_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(blake2b_init)(&ctx->ipad_ctx, BLAKE2B_512_LEN);
	FSYMBOL(blake2b_init)(&ctx->opad_ctx, BLAKE2B_512_LEN);
	FSYMBOL(blake2b_process)(&ctx->ipad_ctx, ctx->ipad, BLAKE2B_BLOCKSIZE);
} /* end */

/* @func: hmac_blake2b_process - hmac-blake2b processing buffer
* @param1: struct hmac_blake2b_ctx * # hmac-blake2b struct context
* @param2: conat uint8 *             # input buffer
* @param3: uint64                    # input length
* @return: void
*/
void FSYMBOL(hmac_blake2b_process)(struct hmac_blake2b_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(blake2b_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_blake2b_finish - hmac-blake2b process the remaining bytes in the \
*                               buffer and end
* @param1: struct hmac_blake2b_ctx * # hmac-blake2b struct context
* @return: void
*/
void FSYMBOL(hmac_blake2b_finish)(struct hmac_blake2b_ctx *ctx) {
	FSYMBOL(blake2b_finish)(&ctx->ipad_ctx);

	FSYMBOL(blake2b_process)(&ctx->opad_ctx, ctx->opad, BLAKE2B_BLOCKSIZE);
	FSYMBOL(blake2b_process)(&ctx->opad_ctx,
		&(BLAKE2B_STATE(&ctx->ipad_ctx, 0)), BLAKE2B_512_LEN);
	FSYMBOL(blake2b_finish)(&ctx->opad_ctx);

	XSYMBOL(memcpy)(ctx->state, &(BLAKE2B_STATE(&ctx->opad_ctx, 0)),
		BLAKE2B_512_LEN);
} /* end */

/* @func: hmac_blake2b - hmac-blake2b processing
* @param1: struct hmac_blake2b_ctx * # hmac-blake2b struct context
* @param2: conat uint8 *             # input buffer
* @param3: uint64                    # input length
* @return: void
*/
void FSYMBOL(hmac_blake2b)(struct hmac_blake2b_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_blake2b_process)(ctx, s, len);
	FSYMBOL(hmac_blake2b_finish)(ctx);
} /* end */
