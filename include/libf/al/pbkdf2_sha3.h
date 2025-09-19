/* pbkdf2_sha3.h - password-based key derivation function (sha3) definitions */

#ifndef LIBF_AL_PBKDF2_SHA256_H
#define LIBF_AL_PBKDF2_SHA256_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* pbkdf2_sha3.c */
extern int32 FSYMBOL(pbkdf2_sha3)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k, int32 type);

#ifdef __cplusplus
}
#endif


#endif
