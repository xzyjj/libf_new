/* x25519.h - curve25519 elliptic-curve crypto (ecc) definitions */

#ifndef LIBF_AL_X25519_H
#define LIBF_AL_X25519_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_512.h>


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
extern int32 FSYMBOL(x25519_mod_inverse)(const bn_int512_t a, const bn_int512_t m,
		bn_int512_t r);
extern void FSYMBOL(x25519_point_add)(const bn_int512_t p,
		const bn_int512_t x1, const bn_int512_t z1,
		const bn_int512_t x2, const bn_int512_t z2,
		const bn_int512_t x_d, const bn_int512_t z_d,
		bn_int512_t r_x, bn_int512_t r_z);
extern void FSYMBOL(x25519_point_double)(const bn_int512_t p, const bn_int512_t a,
		const bn_int512_t x1, const bn_int512_t z1,
		bn_int512_t r_x, bn_int512_t r_z);
extern void FSYMBOL(x25519_scalar_mul)(const bn_int512_t k, const bn_int512_t b,
		const bn_int512_t p, const bn_int512_t a, bn_int512_t r);
extern void FSYMBOL(x25519_shared_key)(const bn_int512_t pri, const bn_int512_t pub,
		const bn_int512_t p, const bn_int512_t a, bn_int512_t key);
extern void FSYMBOL(x25519_clamp_key)(bn_int512_t k);
extern void FSYMBOL(x25519_base_mask)(bn_int512_t b);

#ifdef __cplusplus
}
#endif


#endif
