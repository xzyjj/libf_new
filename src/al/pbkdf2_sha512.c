/* pbkdf2_sha512.c - password-based key derivation function (sha512) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha512.h>
#include <libf/al/pbkdf2_sha512.h>


/* @func: pbkdf2_sha512 - pbkdf2-sha512 key derivation function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @param7: uint32        # iterations number
* @return: void
*/
void FSYMBOL(pbkdf2_sha512)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k) {
	HMAC_SHA512_NEW(ctx);
	uint8 tmp[SHA512_LEN], u[SHA512_LEN], count[4];
	uint32 f = 0;

	uint32 n = (len + SHA512_LEN - 1) / SHA512_LEN;
	for (uint32 i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		FSYMBOL(hmac_sha512_init)(&ctx, pass, pass_len);
		FSYMBOL(hmac_sha512_process)(&ctx, salt, salt_len);
		FSYMBOL(hmac_sha512_process)(&ctx, count, 4);
		FSYMBOL(hmac_sha512_finish)(&ctx, salt_len + 4);
		XSYMBOL(memcpy)(u, &(HMAC_SHA512_STATE(&ctx, 0)), SHA512_LEN);
		XSYMBOL(memcpy)(tmp, &(HMAC_SHA512_STATE(&ctx, 0)), SHA512_LEN);

		for (uint32 j = 1; j < k; j++) {
			FSYMBOL(hmac_sha512_init)(&ctx, pass, pass_len);
			FSYMBOL(hmac_sha512_process)(&ctx, u, SHA512_LEN);
			FSYMBOL(hmac_sha512_finish)(&ctx, SHA512_LEN);
			XSYMBOL(memcpy)(u, &(HMAC_SHA512_STATE(&ctx, 0)),
				SHA512_LEN);

			for (int32 l = 0; l < SHA512_LEN; l++)
				tmp[l] ^= HMAC_SHA512_STATE(&ctx, l);
		}

		XSYMBOL(memcpy)(odk + f, tmp, ((f + SHA512_LEN) > len) ?
			(len % SHA512_LEN) : SHA512_LEN);
		f += SHA512_LEN;
	}
} /* end */

/* @func: pbkdf2_sha384 - pbkdf2-sha384 key derivation function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @param7: uint32        # iterations number
* @return: void
*/
void FSYMBOL(pbkdf2_sha384)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k) {
	HMAC_SHA384_NEW(ctx);
	uint8 tmp[SHA384_LEN], u[SHA384_LEN], count[4];
	uint32 f = 0;

	uint32 n = (len + SHA384_LEN - 1) / SHA384_LEN;
	for (uint32 i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		FSYMBOL(hmac_sha384_init)(&ctx, pass, pass_len);
		FSYMBOL(hmac_sha384_process)(&ctx, salt, salt_len);
		FSYMBOL(hmac_sha384_process)(&ctx, count, 4);
		FSYMBOL(hmac_sha384_finish)(&ctx, salt_len + 4);
		XSYMBOL(memcpy)(u, &(HMAC_SHA384_STATE(&ctx, 0)), SHA384_LEN);
		XSYMBOL(memcpy)(tmp, &(HMAC_SHA384_STATE(&ctx, 0)), SHA384_LEN);

		for (uint32 j = 1; j < k; j++) {
			FSYMBOL(hmac_sha384_init)(&ctx, pass, pass_len);
			FSYMBOL(hmac_sha384_process)(&ctx, u, SHA384_LEN);
			FSYMBOL(hmac_sha384_finish)(&ctx, SHA384_LEN);
			XSYMBOL(memcpy)(u, &(HMAC_SHA384_STATE(&ctx, 0)),
				SHA384_LEN);

			for (int32 l = 0; l < SHA384_LEN; l++)
				tmp[l] ^= HMAC_SHA384_STATE(&ctx, l);
		}

		XSYMBOL(memcpy)(odk + f, tmp, ((f + SHA384_LEN) > len) ?
			(len % SHA384_LEN) : SHA384_LEN);
		f += SHA384_LEN;
	}
} /* end */
