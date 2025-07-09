/* hkdf_sha512.c - hmac-base key derivation function (sha512) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha512.h>
#include <libf/al/hkdf_sha512.h>


/* @func: hkdf_sha512 - hkdf-sha512 key derivation function
* @param1: const uint8 * # hmac-sha512 digest
* @param2: const uint8 * # info (optional)
* @param3: uint32        # info length
* @param4: uint8 *       # output key
* @param5: uint32        # key length
* @return: int32         # 0: no error, -1: key length error
*/
int32 FSYMBOL(hkdf_sha512)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len) {
	HMAC_SHA512_NEW(ctx);
	uint8 tmp[SHA512_LEN];
	uint32 f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * SHA512_LEN))
		return -1;

	uint32 n = (len + SHA512_LEN - 1) / SHA512_LEN;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_sha512_init)(&ctx, prk, SHA512_LEN);
		FSYMBOL(hmac_sha512_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_sha512_process)(&ctx, info, info_len);
		FSYMBOL(hmac_sha512_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_sha512_finish)(&ctx, tmp_len + info_len + 1);
		tmp_len = SHA512_LEN;

		XSYMBOL(memcpy)(tmp, &(HMAC_SHA512_STATE(&ctx, 0)), SHA512_LEN);
		XSYMBOL(memcpy)(okm + f, tmp, SHA512_LEN
			- (((f + SHA512_LEN) % len) % SHA512_LEN));
		f += SHA512_LEN;
	}

	return 0;
} /* end */

/* @func: hkdf_sha384 - hkdf-sha384 key derivation function
* @param1: const uint8 * # hmac-sha384 digest
* @param2: const uint8 * # info (optional)
* @param3: uint32        # info length
* @param4: uint8 *       # output key
* @param5: uint32        # key length
* @return: int32         # 0: no error, -1: key length error
*/
int32 FSYMBOL(hkdf_sha384)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len) {
	HMAC_SHA384_NEW(ctx);
	uint8 tmp[SHA384_LEN];
	uint32 f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * SHA384_LEN))
		return -1;

	uint32 n = (len + SHA384_LEN - 1) / SHA384_LEN;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_sha384_init)(&ctx, prk, SHA384_LEN);
		FSYMBOL(hmac_sha384_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_sha384_process)(&ctx, info, info_len);
		FSYMBOL(hmac_sha384_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_sha384_finish)(&ctx, tmp_len + info_len + 1);
		tmp_len = SHA384_LEN;

		XSYMBOL(memcpy)(tmp, &(HMAC_SHA384_STATE(&ctx, 0)), SHA384_LEN);
		XSYMBOL(memcpy)(okm + f, tmp, SHA384_LEN
			- (((f + SHA384_LEN) % len) % SHA384_LEN));
		f += SHA384_LEN;
	}

	return 0;
} /* end */
