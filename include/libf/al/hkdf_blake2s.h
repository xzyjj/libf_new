/* hkdf_blake2s.h - hmac-base key derivation function (blake2s) definitions */

#ifndef LIBF_AL_HKDF_BLAKE2S_H
#define LIBF_AL_HKDF_BLAKE2S_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* hkdf_blake2s.c */
extern int32 FSYMBOL(hkdf_blake2s)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
