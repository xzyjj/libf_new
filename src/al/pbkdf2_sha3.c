/* pbkdf2_sha3.c - password-based key derivation function (sha3) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha3.h>
#include <libf/al/pbkdf2_sha3.h>


/* @func: pbkdf2_sha3 - pbkdf2-sha3 key derivation function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @param7: uint32        # iterations number
* @return: int32         # 0: no error, -1: digest type error
*/
int32 FSYMBOL(pbkdf2_sha3)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k, int32 type) {
	HMAC_SHA3_NEW(ctx);
	uint8 tmp[SHA3_512_LEN], u[SHA3_512_LEN], count[4];
	uint32 f = 0, dsize = 0;

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

	uint32 n = (len + dsize - 1) / dsize;
	for (uint32 i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		FSYMBOL(hmac_sha3_init)(&ctx, pass, pass_len, type);
		FSYMBOL(hmac_sha3_process)(&ctx, salt, salt_len);
		FSYMBOL(hmac_sha3_process)(&ctx, count, 4);
		FSYMBOL(hmac_sha3_finish)(&ctx);
		XSYMBOL(memcpy)(u, &(HMAC_SHA3_STATE(&ctx, 0)), dsize);
		XSYMBOL(memcpy)(tmp, &(HMAC_SHA3_STATE(&ctx, 0)), dsize);

		for (uint32 j = 1; j < k; j++) {
			FSYMBOL(hmac_sha3_init)(&ctx, pass, pass_len, type);
			FSYMBOL(hmac_sha3_process)(&ctx, u, dsize);
			FSYMBOL(hmac_sha3_finish)(&ctx);
			XSYMBOL(memcpy)(u, &(HMAC_SHA3_STATE(&ctx, 0)),
				dsize);

			for (uint32 l = 0; l < dsize; l++)
				tmp[l] ^= HMAC_SHA3_STATE(&ctx, l);
		}

		XSYMBOL(memcpy)(odk + f, tmp, ((f + dsize) > len) ?
			(len % dsize) : dsize);
		f += dsize;
	}

	return 0;
} /* end */
