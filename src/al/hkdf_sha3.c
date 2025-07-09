/* hkdf_sha3.c - hmac-base key derivation function (sha3) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha3.h>
#include <libf/al/hkdf_sha3.h>


/* @func: hkdf_sha3 - hkdf-sha3 key derivation function
* @param1: const uint8 * # hmac-sha3 digest
* @param2: int32         # digest type
* @param3: const uint8 * # info (optional)
* @param4: uint32        # info length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @return: int32         # 0: no error, -1: digest type error,
*                          -2: key length error
*/
int32 FSYMBOL(hkdf_sha3)(const uint8 *prk, int32 type, const uint8 *info,
		uint32 info_len, uint8 *okm, uint32 len) {
	HMAC_SHA3_NEW(ctx);
	uint8 tmp[SHA3_512_LEN];
	uint32 f = 0, tmp_len = 0, dsize = 0;

	switch (type) {
		case SHA3_224_TYPE:
			dsize = SHA3_224_LEN;
			break;
		case SHA3_256_TYPE:
			dsize = SHA3_256_LEN;
			break;
		case SHA3_384_TYPE:
			dsize = SHA3_384_LEN;
			break;
		case SHA3_512_TYPE:
			dsize = SHA3_512_LEN;
			break;
		default:
			return -1;
	}
	if (len < 1 || len > (255 * dsize))
		return -2;

	uint32 n = (len + dsize - 1) / dsize;
	for (uint32 i = 1; i <= n; i++) {
		FSYMBOL(hmac_sha3_init)(&ctx, prk, dsize, type);
		FSYMBOL(hmac_sha3_process)(&ctx, tmp, tmp_len);
		FSYMBOL(hmac_sha3_process)(&ctx, info, info_len);
		FSYMBOL(hmac_sha3_process)(&ctx, (uint8 *)&i, 1);
		FSYMBOL(hmac_sha3_finish)(&ctx);
		tmp_len = dsize;

		XSYMBOL(memcpy)(tmp, &(HMAC_SHA3_STATE(&ctx, 0)), dsize);
		XSYMBOL(memcpy)(okm + f, tmp, dsize
			- (((f + dsize) % len) % dsize));
		f += dsize;
	}

	return 0;
} /* end */
