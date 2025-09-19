/* hkdf_md5.c - hmac-base key derivation function (md5) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_md5.h>
#include <libf/al/hkdf_md5.h>


/* @func: hkdf_md5 - hkdf-md5 key derivation function
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
int32 FSYMBOL(hkdf_md5)(const uint8 *ikm, uint32 ikm_len,
		const uint8 *salt, uint32 salt_len, const uint8 *info,
		uint32 info_len, uint8 *okm, uint32 len) {
	HMAC_MD5_NEW(ctx);
	uint8 tmp[MD5_LEN], prk[MD5_LEN];
	uint32 f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * MD5_LEN))
		return -1;

	FSYMBOL(hmac_md5_init)(&ctx, salt, salt_len);
	FSYMBOL(hmac_md5_process)(&ctx, ikm, ikm_len);
	FSYMBOL(hmac_md5_finish)(&ctx, ikm_len);
	XSYMBOL(memcpy)(prk, &(HMAC_MD5_STATE(&ctx, 0)), MD5_LEN);

	uint32 n = (len + MD5_LEN - 1) / MD5_LEN;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_md5_init)(&ctx, prk, MD5_LEN);
		FSYMBOL(hmac_md5_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_md5_process)(&ctx, info, info_len);
		FSYMBOL(hmac_md5_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_md5_finish)(&ctx, tmp_len + info_len + 1);
		tmp_len = MD5_LEN;

		XSYMBOL(memcpy)(tmp, &(HMAC_MD5_STATE(&ctx, 0)), MD5_LEN);
		XSYMBOL(memcpy)(okm + f, tmp, ((f + MD5_LEN) > len) ?
			(len % MD5_LEN) : MD5_LEN);
		f += MD5_LEN;
	}

	return 0;
} /* end */
