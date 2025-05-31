/* rsa.h - rsa (rivest–shamir–adleman) cryptosystem definitions */

#ifndef LIBF_AL_RSA_H
#define LIBF_AL_RSA_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_8192.h>


#ifdef __cplusplus
extern "C" {
#endif

/* rsa.c */
extern int32 FSYMBOL(rsa4096_mod_inverse)(const bn_int8192_t a, const bn_int8192_t m,
		bn_int8192_t r);
extern void FSYMBOL(rsa4096_mod_pow)(const bn_int8192_t a, const bn_int8192_t b,
		const bn_int8192_t m, bn_int8192_t r);
extern int32 FSYMBOL(rsa4096_genkey)(const bn_int8192_t p, const bn_int8192_t q,
		bn_int8192_t e, bn_int8192_t d, bn_int8192_t n);
extern int32 FSYMBOL(rsa4096_encrypt)(const bn_int8192_t e, const bn_int8192_t n,
		bn_int8192_t m);
extern int32 FSYMBOL(rsa4096_decrypt)(const bn_int8192_t d, const bn_int8192_t n,
		bn_int8192_t m);

#ifdef __cplusplus
}
#endif


#endif
