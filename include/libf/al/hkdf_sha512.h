/* hkdf_sha512.h - hmac-base key derivation function (sha512) definitions */

#ifndef LIBF_AL_HKDF_SHA512_H
#define LIBF_AL_HKDF_SHA512_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* hkdf_sha512.c */
extern int32 FSYMBOL(hkdf_sha512)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);
extern int32 FSYMBOL(hkdf_sha384)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
