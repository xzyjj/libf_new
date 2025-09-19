/* hmac_sha3.c - hash message authentication code (sha3) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha3.h>
#include <libf/al/sha3.h>


/* @func: hmac_sha3_init - hmac-sha3 key init
* @param1: struct hmac_sha3_ctx * # hmac-sha3 struct context
* @param2: const uint8 *          # input key
* @param3: uint32                 # key length
* @param4: int32                  # digest type
* @return: int32                  # 0: no error, -1: type or init error
*/
int32 FSYMBOL(hmac_sha3_init)(struct hmac_sha3_ctx *ctx, const uint8 *key,
		uint32 key_len, int32 type) {
	SHA3_NEW(sha3_ctx);

	switch (type) {
		case SHA3_224_TYPE: case SHA3_256_TYPE:
		case SHA3_384_TYPE: case SHA3_512_TYPE:
			break;
		default:
			return -1;
	}
	if (FSYMBOL(sha3_init)(&sha3_ctx, type, 0))
		return -1;

	if (key_len > SHA3_GETRATE(&sha3_ctx)) {
		FSYMBOL(sha3)(&sha3_ctx, key, key_len);
		key = &(SHA3_STATE(&sha3_ctx, 0));
		key_len = SHA3_GETDSIZE(&sha3_ctx);
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, SHA3_STATE_SIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, SHA3_STATE_SIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	if (FSYMBOL(sha3_init)(&ctx->ipad_ctx, type, 0))
		return -1;
	if (FSYMBOL(sha3_init)(&ctx->opad_ctx, type, 0))
		return -1;
	FSYMBOL(sha3_process)(&ctx->ipad_ctx, ctx->ipad,
		SHA3_GETRATE(&ctx->ipad_ctx));

	return 0;
} /* end */

/* @func: hmac_sha3_process - hmac-sha3 processing buffer
* @param1: struct hmac_sha3_ctx * # hmac-sha3 struct context
* @param2: conat uint8 *          # input buffer
* @param3: uint64                 # input length
* @return: void
*/
void FSYMBOL(hmac_sha3_process)(struct hmac_sha3_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(sha3_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_sha3_finish - hmac-sha3 process the remaining bytes in the \
*                            buffer and end
* @param1: struct hmac_sha3_ctx * # hmac-sha3 struct context
* @return: void
*/
void FSYMBOL(hmac_sha3_finish)(struct hmac_sha3_ctx *ctx) {
	FSYMBOL(sha3_finish)(&ctx->ipad_ctx);

	FSYMBOL(sha3_process)(&ctx->opad_ctx, ctx->opad,
		SHA3_GETRATE(&ctx->opad_ctx));
	FSYMBOL(sha3_process)(&ctx->opad_ctx,
		&(SHA3_STATE(&ctx->ipad_ctx, 0)),
		SHA3_GETDSIZE(&ctx->ipad_ctx));
	FSYMBOL(sha3_finish)(&ctx->opad_ctx);

	XSYMBOL(memcpy)(ctx->state, &(SHA3_STATE(&ctx->opad_ctx, 0)),
		SHA3_GETDSIZE(&ctx->opad_ctx));
} /* end */

/* @func: hmac_sha3 - hmac-sha3 processing
* @param1: struct hmac_sha3_ctx * # hmac-sha3 struct context
* @param2: conat uint8 *          # input buffer
* @param3: uint64                 # input length
* @return: void
*/
void FSYMBOL(hmac_sha3)(struct hmac_sha3_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_sha3_process)(ctx, s, len);
	FSYMBOL(hmac_sha3_finish)(ctx);
} /* end */
