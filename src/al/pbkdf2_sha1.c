/* pbkdf2_sha1.c - password-based key derivation function (sha1) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha1.h>
#include <libf/al/pbkdf2_sha1.h>


/* @func: pbkdf2_sha1 - pbkdf2-sha1 key derivation function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @param7: uint32        # iterations number
* @return: void
*/
void FSYMBOL(pbkdf2_sha1)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k) {
	HMAC_SHA1_NEW(ctx);
	uint8 tmp[SHA1_LEN], u[SHA1_LEN], count[4];
	uint32 f = 0;

	uint32 n = (len + SHA1_LEN - 1) / SHA1_LEN;
	for (uint32 i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		FSYMBOL(hmac_sha1_init)(&ctx, pass, pass_len);
		FSYMBOL(hmac_sha1_process)(&ctx, salt, salt_len);
		FSYMBOL(hmac_sha1_process)(&ctx, count, 4);
		FSYMBOL(hmac_sha1_finish)(&ctx, salt_len + 4);
		XSYMBOL(memcpy)(u, &(HMAC_SHA1_STATE(&ctx, 0)), SHA1_LEN);
		XSYMBOL(memcpy)(tmp, &(HMAC_SHA1_STATE(&ctx, 0)), SHA1_LEN);

		for (uint32 j = 1; j < k; j++) {
			FSYMBOL(hmac_sha1_init)(&ctx, pass, pass_len);
			FSYMBOL(hmac_sha1_process)(&ctx, u, SHA1_LEN);
			FSYMBOL(hmac_sha1_finish)(&ctx, SHA1_LEN);
			XSYMBOL(memcpy)(u, &(HMAC_SHA1_STATE(&ctx, 0)),
				SHA1_LEN);

			for (int32 l = 0; l < SHA1_LEN; l++)
				tmp[l] ^= HMAC_SHA1_STATE(&ctx, l);
		}

		XSYMBOL(memcpy)(odk + f, tmp, ((f + SHA1_LEN) > len) ?
			(len % SHA1_LEN) : SHA1_LEN);
		f += SHA1_LEN;
	}
} /* end */
