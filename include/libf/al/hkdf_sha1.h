/* hkdf_sha1.h - hmac-base key derivation function (sha1) definitions */

#ifndef LIBF_AL_HKDF_SHA1_H
#define LIBF_AL_HKDF_SHA1_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* hkdf_sha1.c */
extern int32 FSYMBOL(hkdf_sha1)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
