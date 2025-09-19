/* pbkdf2_blake2b.h - password-based key derivation function (blake2b) definitions */

#ifndef LIBF_AL_PBKDF2_BLAKE2B_H
#define LIBF_AL_PBKDF2_BLAKE2B_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* pbkdf2_blake2b.c */
extern void FSYMBOL(pbkdf2_blake2b)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *odk,
		uint32 len, uint32 k);

#ifdef __cplusplus
}
#endif


#endif
