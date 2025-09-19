/* bcrypt.h - bcrypt password-hash definitions */

#ifndef LIBF_AL_BCRYPT_H
#define LIBF_AL_BCRYPT_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bcrypt */
#undef BCRYPT_HASHPASS_LEN
#define BCRYPT_HASHPASS_LEN 24
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bcrypt.c */
extern void FSYMBOL(bcrypt_hashpass)(const uint8 *pass, uint32 pass_len,
		const uint8 *salt, uint32 salt_len, uint8 *ohp, uint32 k);

#ifdef __cplusplus
}
#endif


#endif
