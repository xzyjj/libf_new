/* hkdf_sha256.h - hmac-base key derivation function (sha256) definitions */

#ifndef LIBF_AL_HKDF_SHA256_H
#define LIBF_AL_HKDF_SHA256_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* hkdf_sha256.c */
extern int32 FSYMBOL(hkdf_sha256)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);
extern int32 FSYMBOL(hkdf_sha224)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
