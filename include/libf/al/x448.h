/* x448.h - curve448 elliptic-curve crypto (ecc) definitions */

#ifndef LIBF_AL_X448_H
#define LIBF_AL_X448_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_1024.h>


/* @def: x448
* 2**448 - 2**224 - 1 */
#undef X448_P
#define X448_P "726838724295606890549323807888004534353641360687318060281490199180612328166730772686396383698676545930088884461843637361053498018365439"
#undef X448_A
#define X448_A "156326"
#undef X448_B
#define X448_B "5"

#undef x448_point
struct x448_point {
	bn_int1024_t x;
	bn_int1024_t z;
};

#undef X448_ECDH_NEW
#define X448_ECDH_NEW(x) struct x448_ecdh_ctx x

#undef x448_ecdh_ctx
struct x448_ecdh_ctx {
	bn_uint1024_t p, a, b;
	bn_uint1024_t pri, pub;
	bn_uint1024_t key;
};

#undef X448_LEN
#define X448_LEN 56

#undef X448_PRI
#define X448_PRI(x, n) (((uint8 *)(x)->pri)[n])
#undef X448_PUB
#define X448_PUB(x, n) (((uint8 *)(x)->pub)[n])

#undef X448_KEY
#define X448_KEY(x, n) (((uint8 *)(x)->key)[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* x448.c */
extern int32 FSYMBOL(x448_mod_inverse)(const bn_int1024_t a, const bn_int1024_t m,
		bn_int1024_t r);
extern void FSYMBOL(x448_point_add)(const bn_int1024_t p,
		const struct x448_point *xz1,
		const struct x448_point *xz2,
		const struct x448_point *xzd,
		struct x448_point *r_xz3);
extern void FSYMBOL(x448_point_double)(const bn_int1024_t p, const bn_int1024_t a,
		const struct x448_point *xz1,
		struct x448_point *r_xz3);
extern void FSYMBOL(x448_scalar_mul)(const bn_int1024_t p, const bn_int1024_t a,
		const bn_int1024_t k, const bn_int1024_t b, bn_int1024_t r);
extern void FSYMBOL(x448_clamp_key)(bn_int1024_t k);
extern void FSYMBOL(x448_base_mask)(bn_int1024_t b);

/* x448_ecdh.c */
extern void FSYMBOL(x448_ecdh_init)(struct x448_ecdh_ctx *ctx);
extern void FSYMBOL(x448_ecdh_private)(struct x448_ecdh_ctx *ctx,
		const uint8 *key);
extern void FSYMBOL(x448_ecdh_public)(struct x448_ecdh_ctx *ctx);
extern void FSYMBOL(x448_ecdh_exchange)(struct x448_ecdh_ctx *ctx,
		const uint8 *key);

#ifdef __cplusplus
}
#endif


#endif
