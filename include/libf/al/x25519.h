/* x25519.h - curve25519 elliptic-curve crypto (ecc) definitions */

#ifndef LIBF_AL_X25519_H
#define LIBF_AL_X25519_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_512.h>


/* @def: x25519
* 2*255 - 19 */
#undef X25519_P
#define X25519_P "57896044618658097711785492504343953926634992332820282019728792003956564819949"
#undef X25519_A
#define X25519_A "486662"
#undef X25519_B
#define X25519_B "9"

#undef x25519_point
struct x25519_point {
	bn_int512_t x;
	bn_int512_t z;
};

#undef X25519_ECDH_NEW
#define X25519_ECDH_NEW(x) struct x25519_ecdh_ctx x

#undef x25519_ecdh_ctx
struct x25519_ecdh_ctx {
	bn_uint512_t p, a, b;
	bn_uint512_t pri, pub;
	bn_uint512_t key;
};

#undef X25519_LEN
#define X25519_LEN 32

#undef X25519_PRI
#define X25519_PRI(x, n) (((uint8 *)(x)->pri)[n])
#undef X25519_PUB
#define X25519_PUB(x, n) (((uint8 *)(x)->pub)[n])

#undef X25519_KEY
#define X25519_KEY(x, n) (((uint8 *)(x)->key)[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* x25519.c */
extern int32 FSYMBOL(x25519_mod_inverse)(const bn_int512_t a, const bn_int512_t m,
		bn_int512_t r);
extern void FSYMBOL(x25519_point_add)(const bn_int512_t p,
		const struct x25519_point *xz1,
		const struct x25519_point *xz2,
		const struct x25519_point *xzd,
		struct x25519_point *r_xz3);
extern void FSYMBOL(x25519_point_double)(const bn_int512_t p, const bn_int512_t a,
		const struct x25519_point *xz1,
		struct x25519_point *r_xz3);
extern void FSYMBOL(x25519_scalar_mul)(const bn_int512_t p, const bn_int512_t a,
		const bn_int512_t k, const bn_int512_t b, bn_int512_t r);
extern void FSYMBOL(x25519_clamp_key)(bn_int512_t k);
extern void FSYMBOL(x25519_base_mask)(bn_int512_t b);

/* x25519_ecdh.c */
extern void FSYMBOL(x25519_ecdh_init)(struct x25519_ecdh_ctx *ctx);
extern void FSYMBOL(x25519_ecdh_private)(struct x25519_ecdh_ctx *ctx,
		const uint8 *key);
extern void FSYMBOL(x25519_ecdh_public)(struct x25519_ecdh_ctx *ctx);
extern void FSYMBOL(x25519_ecdh_exchange)(struct x25519_ecdh_ctx *ctx,
		const uint8 *key);

#ifdef __cplusplus
}
#endif


#endif
