/* hkdf_md5.h - hmac-base key derivation function (md5) definitions */

#ifndef LIBF_AL_HKDF_MD5_H
#define LIBF_AL_HKDF_MD5_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* hkdf_md5.c */
extern int32 FSYMBOL(hkdf_md5)(const uint8 *prk, const uint8 *info, uint32 info_len,
		uint8 *okm, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
