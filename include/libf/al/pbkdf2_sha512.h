/* pbkdf2_sha512.h - password-based key derivation function (sha512) definitions */

#ifndef LIBF_AL_PBKDF2_SHA512_H
#define LIBF_AL_PBKDF2_SHA512_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* pbkdf2_sha512.c */
extern void FSYMBOL(pbkdf2_sha512)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k);
extern void FSYMBOL(pbkdf2_sha384)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k);

#ifdef __cplusplus
}
#endif


#endif
