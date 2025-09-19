/* pbkdf2_sha256.h - password-based key derivation function (sha256) definitions */

#ifndef LIBF_AL_PBKDF2_SHA256_H
#define LIBF_AL_PBKDF2_SHA256_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* pbkdf2_sha256.c */
extern void FSYMBOL(pbkdf2_sha256)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k);
extern void FSYMBOL(pbkdf2_sha224)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k);

#ifdef __cplusplus
}
#endif


#endif
