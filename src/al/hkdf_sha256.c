/* hkdf_sha256.c - hmac-base key derivation function (sha256) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha256.h>
#include <libf/al/hkdf_sha256.h>


/* @func: hkdf_sha256 - hkdf-sha256 key derivation function
* @param1: const uint8 * # ikm
* @param2: uint32        # ikm length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: const uint8 * # info
* @param6: uint32        # info length
* @param7: uint8 *       # output key
* @param8: uint32        # key length
* @return: int32         # 0: no error, -1: key length error
*/
int32 FSYMBOL(hkdf_sha256)(const uint8 *ikm, uint32 ikm_len,
		const uint8 *salt, uint32 salt_len, const uint8 *info,
		uint32 info_len, uint8 *okm, uint32 len) {
	HMAC_SHA256_NEW(ctx);
	uint8 tmp[SHA256_LEN], prk[SHA256_LEN];
	uint32 f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * SHA256_LEN))
		return -1;

	FSYMBOL(hmac_sha256_init)(&ctx, salt, salt_len);
	FSYMBOL(hmac_sha256_process)(&ctx, ikm, ikm_len);
	FSYMBOL(hmac_sha256_finish)(&ctx, ikm_len);
	XSYMBOL(memcpy)(prk, &(HMAC_SHA256_STATE(&ctx, 0)), SHA256_LEN);

	uint32 n = (len + SHA256_LEN - 1) / SHA256_LEN;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_sha256_init)(&ctx, prk, SHA256_LEN);
		FSYMBOL(hmac_sha256_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_sha256_process)(&ctx, info, info_len);
		FSYMBOL(hmac_sha256_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_sha256_finish)(&ctx, tmp_len + info_len + 1);
		tmp_len = SHA256_LEN;

		XSYMBOL(memcpy)(tmp, &(HMAC_SHA256_STATE(&ctx, 0)), SHA256_LEN);
		XSYMBOL(memcpy)(okm + f, tmp, ((f + SHA256_LEN) > len) ?
			(len % SHA256_LEN) : SHA256_LEN);
		f += SHA256_LEN;
	}

	return 0;
} /* end */

/* @func: hkdf_sha224 - hkdf-sha224 key derivation function
* @param1: const uint8 * # ikm
* @param2: uint32        # ikm length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: const uint8 * # info
* @param6: uint32        # info length
* @param7: uint8 *       # output key
* @param8: uint32        # key length
* @return: int32         # 0: no error, -1: key length error
*/
int32 FSYMBOL(hkdf_sha224)(const uint8 *ikm, uint32 ikm_len,
		const uint8 *salt, uint32 salt_len, const uint8 *info,
		uint32 info_len, uint8 *okm, uint32 len) {
	HMAC_SHA224_NEW(ctx);
	uint8 tmp[SHA224_LEN], prk[SHA224_LEN];
	uint32 f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * SHA224_LEN))
		return -1;

	FSYMBOL(hmac_sha224_init)(&ctx, salt, salt_len);
	FSYMBOL(hmac_sha224_process)(&ctx, ikm, ikm_len);
	FSYMBOL(hmac_sha224_finish)(&ctx, ikm_len);
	XSYMBOL(memcpy)(prk, &(HMAC_SHA224_STATE(&ctx, 0)), SHA224_LEN);

	uint32 n = (len + SHA224_LEN - 1) / SHA224_LEN;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_sha224_init)(&ctx, prk, SHA224_LEN);
		FSYMBOL(hmac_sha224_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_sha224_process)(&ctx, info, info_len);
		FSYMBOL(hmac_sha224_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_sha224_finish)(&ctx, tmp_len + info_len + 1);
		tmp_len = SHA224_LEN;

		XSYMBOL(memcpy)(tmp, &(HMAC_SHA224_STATE(&ctx, 0)), SHA224_LEN);
		XSYMBOL(memcpy)(okm + f, tmp, ((f + SHA224_LEN) > len) ?
			(len % SHA224_LEN) : SHA224_LEN);
		f += SHA224_LEN;
	}

	return 0;
} /* end */
