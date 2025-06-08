/* hmac_md5.c - hash message authentication code (md5) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_md5.h>
#include <libf/al/md5.h>


/* @func: hmac_md5_init - hmac-md5 key init
* @param1: struct hmac_md5_ctx * # hmac-md5 struct context
* @param2: const uint8 *         # input key
* @param3: uint32                # key length
* @return: void
*/
void FSYMBOL(hmac_md5_init)(struct hmac_md5_ctx *ctx, const uint8 *key,
		uint32 key_len) {
	MD5_NEW(md5_ctx);
	if (key_len > MD5_BLOCKSIZE) {
		FSYMBOL(md5)(&md5_ctx, key, key_len);
		key = &(MD5_STATE(&md5_ctx, 0));
		key_len = MD5_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, MD5_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, MD5_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	FSYMBOL(md5_init)(&ctx->ipad_ctx);
	FSYMBOL(md5_init)(&ctx->opad_ctx);
	FSYMBOL(md5_process)(&ctx->ipad_ctx, ctx->ipad, MD5_BLOCKSIZE);
} /* end */

/* @func: hmac_md5_process - hmac-md5 processing buffer
* @param1: struct hmac_md5_ctx * # hmac-md5 struct context
* @param2: conat uint8 *         # input buffer
* @param3: uint64                # input length
* @return: void
*/
void FSYMBOL(hmac_md5_process)(struct hmac_md5_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(md5_process)(&ctx->ipad_ctx, s, len);
} /* end */

/* @func: hmac_md5_finish - hmac-md5 process the remaining bytes in the \
*                           buffer and end
* @param1: struct hmac_md5_ctx * # hmac-md5 struct context
* @param2: uint64L               # total length
* @return: void
*/
void FSYMBOL(hmac_md5_finish)(struct hmac_md5_ctx *ctx, uint64L len) {
	FSYMBOL(md5_finish)(&ctx->ipad_ctx, len + MD5_BLOCKSIZE);

	FSYMBOL(md5_process)(&ctx->opad_ctx, ctx->opad, MD5_BLOCKSIZE);
	FSYMBOL(md5_process)(&ctx->opad_ctx, &(MD5_STATE(&ctx->ipad_ctx, 0)),
		MD5_LEN);
	FSYMBOL(md5_finish)(&ctx->opad_ctx, MD5_BLOCKSIZE + MD5_LEN);

	XSYMBOL(memcpy)(ctx->state, &(MD5_STATE(&ctx->opad_ctx, 0)), MD5_LEN);
} /* end */

/* @func: hmac_md5 - hmac-md5 processing
* @param1: struct hmac_md5_ctx * # hmac-md5 struct context
* @param2: conat uint8 *         # input buffer
* @param3: uint64                # input length
* @return: void
*/
void FSYMBOL(hmac_md5)(struct hmac_md5_ctx *ctx, const uint8 *s,
		uint64 len) {
	FSYMBOL(hmac_md5_process)(ctx, s, len);
	FSYMBOL(hmac_md5_finish)(ctx, len);
} /* end */
