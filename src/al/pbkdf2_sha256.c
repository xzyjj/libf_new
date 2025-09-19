/* pbkdf2_sha256.c - password-based key derivation function (sha256) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha256.h>
#include <libf/al/pbkdf2_sha256.h>


/* @func: pbkdf2_sha256 - pbkdf2-sha256 key derivation function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @param7: uint32        # iterations number
* @return: void
*/
void FSYMBOL(pbkdf2_sha256)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k) {
	HMAC_SHA256_NEW(ctx);
	uint8 tmp[SHA256_LEN], u[SHA256_LEN], count[4];
	uint32 f = 0;

	uint32 n = (len + SHA256_LEN - 1) / SHA256_LEN;
	for (uint32 i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		FSYMBOL(hmac_sha256_init)(&ctx, pass, pass_len);
		FSYMBOL(hmac_sha256_process)(&ctx, salt, salt_len);
		FSYMBOL(hmac_sha256_process)(&ctx, count, 4);
		FSYMBOL(hmac_sha256_finish)(&ctx, salt_len + 4);
		XSYMBOL(memcpy)(u, &(HMAC_SHA256_STATE(&ctx, 0)), SHA256_LEN);
		XSYMBOL(memcpy)(tmp, &(HMAC_SHA256_STATE(&ctx, 0)), SHA256_LEN);

		for (uint32 j = 1; j < k; j++) {
			FSYMBOL(hmac_sha256_init)(&ctx, pass, pass_len);
			FSYMBOL(hmac_sha256_process)(&ctx, u, SHA256_LEN);
			FSYMBOL(hmac_sha256_finish)(&ctx, SHA256_LEN);
			XSYMBOL(memcpy)(u, &(HMAC_SHA256_STATE(&ctx, 0)),
				SHA256_LEN);

			for (int32 l = 0; l < SHA256_LEN; l++)
				tmp[l] ^= HMAC_SHA256_STATE(&ctx, l);
		}

		XSYMBOL(memcpy)(odk + f, tmp, ((f + SHA256_LEN) > len) ?
			(len % SHA256_LEN) : SHA256_LEN);
		f += SHA256_LEN;
	}
} /* end */

/* @func: pbkdf2_sha224 - pbkdf2-sha224 key derivation function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @param7: uint32        # iterations number
* @return: void
*/
void FSYMBOL(pbkdf2_sha224)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k) {
	HMAC_SHA224_NEW(ctx);
	uint8 tmp[SHA224_LEN], u[SHA224_LEN], count[4];
	uint32 f = 0;

	uint32 n = (len + SHA224_LEN - 1) / SHA224_LEN;
	for (uint32 i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		FSYMBOL(hmac_sha224_init)(&ctx, pass, pass_len);
		FSYMBOL(hmac_sha224_process)(&ctx, salt, salt_len);
		FSYMBOL(hmac_sha224_process)(&ctx, count, 4);
		FSYMBOL(hmac_sha224_finish)(&ctx, salt_len + 4);
		XSYMBOL(memcpy)(u, &(HMAC_SHA224_STATE(&ctx, 0)), SHA224_LEN);
		XSYMBOL(memcpy)(tmp, &(HMAC_SHA224_STATE(&ctx, 0)), SHA224_LEN);

		for (uint32 j = 1; j < k; j++) {
			FSYMBOL(hmac_sha224_init)(&ctx, pass, pass_len);
			FSYMBOL(hmac_sha224_process)(&ctx, u, SHA224_LEN);
			FSYMBOL(hmac_sha224_finish)(&ctx, SHA224_LEN);
			XSYMBOL(memcpy)(u, &(HMAC_SHA224_STATE(&ctx, 0)),
				SHA224_LEN);

			for (int32 l = 0; l < SHA224_LEN; l++)
				tmp[l] ^= HMAC_SHA224_STATE(&ctx, l);
		}

		XSYMBOL(memcpy)(odk + f, tmp, ((f + SHA224_LEN) > len) ?
			(len % SHA224_LEN) : SHA224_LEN);
		f += SHA224_LEN;
	}
} /* end */
