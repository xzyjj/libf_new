/* ed25519_ref.h - edwards-curve digital signature algorithm (eddsa) definitions */

#ifndef LIBF_AL_ED25519_REF_H
#define LIBF_AL_ED25519_REF_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_512.h>


/* @def: ed25519
* 2**255 - 19  */
#undef ED25519_P
#define ED25519_P "57896044618658097711785492504343953926634992332820282019728792003956564819949"

/* -121665 * inv(121666) % P */
#undef ED25519_D
#define ED25519_D "37095705934669439343138083508754565189542113879843219016388785533085940283555"

/* 2**252 + 27742317777372353535851937790883648493 */
#undef ED25519_Q
#define ED25519_Q "7237005577332262213973186563042994240857116359379907606001950938285454250989"

/* 4 * inv(5) % p */
#define ED25519_By "46316835694926478169428394003475163141307993866256225615783033603165251855960"

/* rec_x(by, 0) */
#undef ED25519_Bx
#define ED25519_Bx "15112221349535400772501151409588531511454012693041857206046113283949847762202"

#undef ED25519_Bx1
#define ED25519_Bx1 "15112221349535400772501151409588531511454012693041857206046113283949847762202"
#undef ED25519_By1
#define ED25519_By1 "46316835694926478169428394003475163141307993866256225615783033603165251855960"
#undef ED25519_Bz1
#define ED25519_Bz1 "1"
/* bx * by % p */
#undef ED25519_Bt1
#define ED25519_Bt1 "46827403850823179245072216630277197565144205554125654976674165829533817101731"

#undef ed25519_point
struct ed25519_point {
	bn_int512_t x;
	bn_int512_t y;
	bn_int512_t z;
	bn_int512_t t;
};

#undef ED25519_SIGN_NEW
#define ED25519_SIGN_NEW(x) struct ed25519_sign_ctx x

#undef ed25519_sign_ctx
struct ed25519_sign_ctx {
	bn_int512_t p, d, q;
	struct ed25519_point xyz1;
	bn_int512_t pri, ran, pub;
	bn_int512_t rs, ss;
};

#undef ED25519_LEN
#define ED25519_LEN 32

#undef ED25519_PRI
#define ED25519_PRI(x, n) (((uint8 *)(x)->pri)[n])
#undef ED25519_PUB
#define ED25519_PUB(x, n) (((uint8 *)(x)->pub)[n])

#undef ED25519_SIGN_1
#define ED25519_SIGN_1(x, n) (((uint8 *)(x)->rs)[n])
#undef ED25519_SIGN_2
#define ED25519_SIGN_2(x, n) (((uint8 *)(x)->ss)[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* ed25519.c */
extern int32 FSYMBOL(ed25519_mod_inverse)(const bn_int512_t a, const bn_int512_t m,
		bn_int512_t r);
extern void FSYMBOL(ed25519_mod_pow)(const bn_int512_t a, const bn_int512_t b,
		const bn_int512_t m, bn_int512_t r);
extern void FSYMBOL(ed25519_point_add)(const bn_int512_t p, const bn_int512_t d,
		const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2,
		struct ed25519_point *r_xyz3);
extern void FSYMBOL(ed25519_point_double)(const bn_int512_t p, const bn_int512_t d,
		const struct ed25519_point *xyz1,
		struct ed25519_point *r_xyz3);
extern void FSYMBOL(ed25519_scalar_mul)(const bn_int512_t p, const bn_int512_t d,
		const bn_int512_t k, const struct ed25519_point *xyz1,
		struct ed25519_point *r_xyz2);
extern int32 FSYMBOL(ed25519_point_equal)(const bn_int512_t p,
		const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2);
extern void FSYMBOL(ed25519_point_recover_x)(const bn_int512_t p, const bn_int512_t d,
		const bn_int512_t y, uint32 sign, bn_int512_t r);
extern void FSYMBOL(ed25519_point_compress)(const bn_int512_t p,
		const struct ed25519_point *xyz1, bn_int512_t r);
extern void FSYMBOL(ed25519_point_decompress)(const bn_int512_t p, const bn_int512_t d,
		const bn_int512_t k, struct ed25519_point *r_xyz1);
extern void FSYMBOL(ed25519_clamp_key)(bn_int512_t k);

/* ed25519_sign.c */
extern void FSYMBOL(ed25519_sign_init)(struct ed25519_sign_ctx *ctx);
extern void FSYMBOL(ed25519_sign_private)(struct ed25519_sign_ctx *ctx,
		const uint8 *key);
extern void FSYMBOL(ed25519_sign_public)(struct ed25519_sign_ctx *ctx);
extern void FSYMBOL(ed25519_sign)(struct ed25519_sign_ctx *ctx,
		const uint8 *mesg, uint32 len);
extern int32 FSYMBOL(ed25519_sign_verify)(struct ed25519_sign_ctx *ctx,
		const uint8 *key, const uint8 *sign,
		const uint8 *mesg, uint32 len);

#ifdef __cplusplus
}
#endif


#endif
