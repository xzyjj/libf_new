/* hkdf_blake2b.h - hmac-base key derivation function (blake2b) definitions */

#ifndef LIBF_AL_HKDF_BLAKE2B_H
#define LIBF_AL_HKDF_BLAKE2B_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* hkdf_blake2b.c */
extern int32 FSYMBOL(hkdf_blake2b)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
