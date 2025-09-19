/* hkdf_sha1.c - hmac-base key derivation function (sha1) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha1.h>
#include <libf/al/hkdf_sha1.h>


/* @func: hkdf_sha1 - hkdf-sha1 key derivation function
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
int32 FSYMBOL(hkdf_sha1)(const uint8 *ikm, uint32 ikm_len,
		const uint8 *salt, uint32 salt_len, const uint8 *info,
		uint32 info_len, uint8 *okm, uint32 len) {
	HMAC_SHA1_NEW(ctx);
	uint8 tmp[SHA1_LEN], prk[SHA1_LEN];
	uint32 f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * SHA1_LEN))
		return -1;

	FSYMBOL(hmac_sha1_init)(&ctx, salt, salt_len);
	FSYMBOL(hmac_sha1_process)(&ctx, ikm, ikm_len);
	FSYMBOL(hmac_sha1_finish)(&ctx, ikm_len);
	XSYMBOL(memcpy)(prk, &(HMAC_SHA1_STATE(&ctx, 0)), SHA1_LEN);

	uint32 n = (len + SHA1_LEN - 1) / SHA1_LEN;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_sha1_init)(&ctx, prk, SHA1_LEN);
		FSYMBOL(hmac_sha1_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_sha1_process)(&ctx, info, info_len);
		FSYMBOL(hmac_sha1_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_sha1_finish)(&ctx, tmp_len + info_len + 1);
		tmp_len = SHA1_LEN;

		XSYMBOL(memcpy)(tmp, &(HMAC_SHA1_STATE(&ctx, 0)), SHA1_LEN);
		XSYMBOL(memcpy)(okm + f, tmp, ((f + SHA1_LEN) > len) ?
			(len % SHA1_LEN) : SHA1_LEN);
		f += SHA1_LEN;
	}

	return 0;
} /* end */
