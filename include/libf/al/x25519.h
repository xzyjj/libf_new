/* x25519.h - curve25519 elliptic-curve crypto definitions */

#ifndef LIBF_AL_X25519_H
#define LIBF_AL_X25519_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_1024.h>


/* @def: x25519 */
#undef X25519_P
#define X25519_P "57896044618658097711785492504343953926634992332820282019728792003956564819949"
#undef X25519_A
#define X25519_A "486662"
#undef X25519_B
#define X25519_B "9"
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* x25519.c */
extern int32 FSYMBOL(x25519_mod_inverse)(const bn_int1024_t a, const bn_int1024_t m,
		bn_int1024_t r);
extern void FSYMBOL(x25519_scalar_mul)(const bn_int1024_t k, const bn_int1024_t b,
		const bn_int1024_t p, const bn_int1024_t a, bn_int1024_t r);
extern void FSYMBOL(x25519_shared_key)(const bn_int1024_t pri, const bn_int1024_t pub,
		const bn_int1024_t p, const bn_int1024_t a, bn_int1024_t key);
extern void FSYMBOL(x25519_clamp_key)(bn_int1024_t k);

#ifdef __cplusplus
}
#endif


#endif
