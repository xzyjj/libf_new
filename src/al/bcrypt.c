/* bcrypt.c - bcrypt password-hash implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/blowfish.h>
#include <libf/al/bcrypt.h>


/* @func: bcrypt_hashpass - bcrypt password-hashing function
* @param1: const uint8 * # password
* @param2: uint32        # password length
* @param3: const uint8 * # salt
* @param4: uint32        # salt length
* @param5: uint8 *       # output hashpass (length: BCRYPT_HASHPASS_LEN)
* @param6: uint32        # cost (2^N)
* @return: void
*/
void FSYMBOL(bcrypt_hashpass)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *ohp, uint32 k) {
	BLOWFISH_NEW(ctx);
	uint32 n = 1U << k;

	FSYMBOL(blowfish_init)(&ctx, NULL, 0);
	FSYMBOL(blowfish_ekskey)(&ctx, salt, salt_len, pass, pass_len);

	for (uint32 i = 0; i < n; i++) {
		FSYMBOL(blowfish_setkey)(&ctx, pass, pass_len);
		FSYMBOL(blowfish_setkey)(&ctx, salt, salt_len);
	}

	uint32 cdata[6] = { /* "OrpheanBeholderScryDoubt" 24-byte */
		0x4f727068, 0x65616e42, 0x65686f6c, 0x64657253,
		0x63727944, 0x6f756274
		};
	
	for (int32 i = 0; i < 64; i++) {
		FSYMBOL(blowfish_encrypt)(&ctx, &cdata[0], &cdata[1]);
		FSYMBOL(blowfish_encrypt)(&ctx, &cdata[2], &cdata[3]);
		FSYMBOL(blowfish_encrypt)(&ctx, &cdata[4], &cdata[5]);
	}

	for (int32 i = 0; i < 6; i++) {
		ohp[4 * i] = (cdata[i] >> 24) & 0xff;
		ohp[4 * i + 1] = (cdata[i] >> 16) & 0xff;
		ohp[4 * i + 2] = (cdata[i] >> 8) & 0xff;
		ohp[4 * i + 3] = cdata[i] & 0xff;
	}
} /* end */
