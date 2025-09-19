/* rsa.h - definitions */

#ifndef LIBF_AL_RSA_H
#define LIBF_AL_RSA_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_8193.h>


#ifdef __cplusplus
extern "C" {
#endif

/* rsa.c */
extern int32 FSYMBOL(rsa4096_genkey)(const bn_8193_t p, const bn_8193_t q,
		bn_8193_t e, bn_8193_t d, bn_8193_t n);
extern int32 FSYMBOL(rsa4096_encrypt)(const bn_8193_t e, const bn_8193_t n,
		bn_8193_t m);
extern int32 FSYMBOL(rsa4096_decrypt)(const bn_8193_t d, const bn_8193_t n,
		bn_8193_t m);
extern int32 FSYMBOL(rsa4096_gencrt)(const bn_8193_t p, const bn_8193_t q,
		const bn_8193_t d,
		bn_8193_t dp, bn_8193_t dq, bn_8193_t qinv);
extern int32 FSYMBOL(rsa4096_crt_decrypt)(const bn_8193_t q, const bn_8193_t p,
		const bn_8193_t dp, const bn_8193_t dq, const bn_8193_t qinv,
		bn_8193_t m);

#ifdef __cplusplus
}
#endif


#endif
