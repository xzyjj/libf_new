/* pbkdf2_blake2s.c - password-based key derivation function (blake2s) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_blake2s.h>
#include <libf/al/pbkdf2_blake2s.h>


/* @func: pbkdf2_blake2s - pbkdf2-blake2s key derivation function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output key
* @param6: uint32        # key length
* @param7: uint32        # iterations number
* @return: void
*/
void FSYMBOL(pbkdf2_blake2s)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k) {
	HMAC_BLAKE2S_NEW(ctx);
	uint8 tmp[BLAKE2S_256_LEN], u[BLAKE2S_256_LEN], count[4];
	uint32 f = 0;

	uint32 n = (len + BLAKE2S_256_LEN - 1) / BLAKE2S_256_LEN;
	for (uint32 i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		FSYMBOL(hmac_blake2s_init)(&ctx, pass, pass_len);
		FSYMBOL(hmac_blake2s_process)(&ctx, salt, salt_len);
		FSYMBOL(hmac_blake2s_process)(&ctx, count, 4);
		FSYMBOL(hmac_blake2s_finish)(&ctx);
		XSYMBOL(memcpy)(u, &(HMAC_BLAKE2S_STATE(&ctx, 0)),
			BLAKE2S_256_LEN);
		XSYMBOL(memcpy)(tmp, &(HMAC_BLAKE2S_STATE(&ctx, 0)),
			BLAKE2S_256_LEN);

		for (uint32 j = 1; j < k; j++) {
			FSYMBOL(hmac_blake2s_init)(&ctx, pass, pass_len);
			FSYMBOL(hmac_blake2s_process)(&ctx, u, BLAKE2S_256_LEN);
			FSYMBOL(hmac_blake2s_finish)(&ctx);
			XSYMBOL(memcpy)(u, &(HMAC_BLAKE2S_STATE(&ctx, 0)),
				BLAKE2S_256_LEN);

			for (int32 l = 0; l < BLAKE2S_256_LEN; l++)
				tmp[l] ^= HMAC_BLAKE2S_STATE(&ctx, l);
		}

		XSYMBOL(memcpy)(odk + f, tmp, ((f + BLAKE2S_256_LEN) > len) ?
			(len % BLAKE2S_256_LEN) : BLAKE2S_256_LEN);
		f += BLAKE2S_256_LEN;
	}
} /* end */
