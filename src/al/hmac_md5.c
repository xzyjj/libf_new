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
	if (key_len > HAMC_MD5_BLOCKSIZE) {
		FSYMBOL(md5)(&md5_ctx, key, key_len);
		key = &(MD5_STATE(&md5_ctx, 0));
		key_len = MD5_LEN;
	}

	XSYMBOL(memset)(ctx->ipad, 0x36, HAMC_MD5_BLOCKSIZE);
	XSYMBOL(memset)(ctx->opad, 0x5c, HAMC_MD5_BLOCKSIZE);
	for (uint32 i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}
} /* end */

/* @func: hmac_md5 - hmac-md5 digest function
* @param1: struct hmac_md5_ctx * # hmac-md5 struct context
* @param2: const uint8 *         # input message
* @param3: uint32                # message length
* @return: void
*/
void FSYMBOL(hmac_md5)(struct hmac_md5_ctx *ctx, const uint8 *s,
		uint32 len) {
	MD5_NEW(md5_ipad_ctx);
	MD5_NEW(md5_opad_ctx);

	FSYMBOL(md5_process)(&md5_ipad_ctx, ctx->ipad, HAMC_MD5_BLOCKSIZE);
	FSYMBOL(md5_process)(&md5_ipad_ctx, s, len);
	FSYMBOL(md5_finish)(&md5_ipad_ctx, len + HAMC_MD5_BLOCKSIZE);

	FSYMBOL(md5_process)(&md5_opad_ctx, ctx->opad, HAMC_MD5_BLOCKSIZE);
	FSYMBOL(md5_process)(&md5_opad_ctx, &(MD5_STATE(&md5_ipad_ctx, 0)),
		MD5_LEN);
	FSYMBOL(md5_finish)(&md5_opad_ctx, HAMC_MD5_BLOCKSIZE + MD5_LEN);

	XSYMBOL(memcpy)(ctx->state, &(MD5_STATE(&md5_opad_ctx, 0)), MD5_LEN);
} /* end */
