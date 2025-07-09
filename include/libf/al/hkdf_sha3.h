/* hkdf_sha3.h - hmac-base key derivation function (sha3) definitions */

#ifndef LIBF_AL_HKDF_SHA3_H
#define LIBF_AL_HKDF_SHA3_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* hkdf_sha3.c */
extern int32 FSYMBOL(hkdf_sha3)(const uint8 *prk, int32 type, const uint8 *info,
		uint32 info_len, uint8 *okm, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
