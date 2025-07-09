/* hkdf_blake2b.c - hmac-base key derivation function (blake2b) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_blake2b.h>
#include <libf/al/hkdf_blake2b.h>


/* @func: hkdf_blake2b - hkdf-blake2b key derivation function
* @param1: const uint8 * # hmac-blake2b digest
* @param2: const uint8 * # info (optional)
* @param3: uint32        # info length
* @param4: uint8 *       # output key
* @param5: uint32        # key length
* @return: int32         # 0: no error, -1: key length error
*/
int32 FSYMBOL(hkdf_blake2b)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len) {
	HMAC_BLAKE2B_NEW(ctx);
	uint8 tmp[BLAKE2B_512_LEN];
	uint32 f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * BLAKE2B_512_LEN))
		return -1;

	uint32 n = (len + BLAKE2B_512_LEN - 1) / BLAKE2B_512_LEN;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_blake2b_init)(&ctx, prk, BLAKE2B_512_LEN);
		FSYMBOL(hmac_blake2b_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_blake2b_process)(&ctx, info, info_len);
		FSYMBOL(hmac_blake2b_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_blake2b_finish)(&ctx);
		tmp_len = BLAKE2B_512_LEN;

		XSYMBOL(memcpy)(tmp, &(HMAC_BLAKE2B_STATE(&ctx, 0)),
			BLAKE2B_512_LEN);
		XSYMBOL(memcpy)(okm + f, tmp, BLAKE2B_512_LEN
			- (((f + BLAKE2B_512_LEN) % len) % BLAKE2B_512_LEN));
		f += BLAKE2B_512_LEN;
	}

	return 0;
} /* end */
