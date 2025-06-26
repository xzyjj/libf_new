/* ed25519_fast.h - edwards-curve digital signature algorithm (eddsa) definitions */

#ifndef LIBF_AL_ED25519_FAST_H
#define LIBF_AL_ED25519_FAST_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: ed25519_fast */
#undef ED25519_FAST_P
#define ED25519_FAST_P _ed25519_fast_p
#undef ED25519_FAST_D
#define ED25519_FAST_D _ed25519_fast_d
#undef ED25519_FAST_ONE
#define ED25519_FAST_ONE _ed25519_fast_one
#undef ED25519_FAST_P38
#define ED25519_FAST_P38 _ed25519_fast_p38
#undef ED25519_FAST_P14
#define ED25519_FAST_P14 _ed25519_fast_p14
#undef ED25519_FAST_Q
#define ED25519_FAST_Q _ed25519_fast_q
#undef ED25519_FAST_BX
#define ED25519_FAST_BX _ed25519_fast_bx
#undef ED25519_FAST_BY
#define ED25519_FAST_BY _ed25519_fast_by
#undef ED25519_FAST_BZ
#define ED25519_FAST_BZ _ed25519_fast_bz
#undef ED25519_FAST_BT
#define ED25519_FAST_BT _ed25519_fast_bt

/* P = 2**255 - 19 */
static const uint32 _ed25519_fast_p[8] = {
	0xffffffed, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0x7fffffff
	};

/* D = -121665 * inv(121666) % P */
static const uint32 _ed25519_fast_d[8] = {
	0x135978a3, 0x75eb4dca, 0x4141d8ab, 0x00700a4d,
	0x7779e898, 0x8cc74079, 0x2b6ffe73, 0x52036cee
	};

/* one = 1 */
static const uint32 _ed25519_fast_one[8] = {
	1, 0, 0, 0, 0, 0, 0, 0
	};

/* p38 = (P + 3) / 8 */
static const uint32 _ed25519_fast_p38[8] = {
	0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0x0fffffff,
	};

/* p14 = modpow((P - 1) / 4, P) */
static const uint32 _ed25519_fast_p14[8] = {
	0x4a0ea0b0, 0xc4ee1b27, 0xad2fe478, 0x2f431806,
	0x3dfbd7a7, 0x2b4d0099, 0x4fc1df0b, 0x2b832480,
	};

/* Q = 2**252 + 27742317777372353535851937790883648493 */
static const uint32 _ed25519_fast_q[8] = {
	0x5cf5d3ed, 0x5812631a, 0xa2f79cd6, 0x14def9de,
	0, 0, 0, 0x10000000
	};

/* Bx = rec_x(By, 0)
*       15112221349535400772501151409588531511454012693041857206046113283949847762202 */
static const uint32 _ed25519_fast_bx[8] = {
	0x8f25d51a, 0xc9562d60, 0x9525a7b2, 0x692cc760,
	0xfdd6dc5c, 0xc0a4e231, 0xcd6e53fe, 0x216936d3
	};

/* By = 46316835694926478169428394003475163141307993866256225615783033603165251855960 */
static const uint32 _ed25519_fast_by[8] = {
	0x66666658, 0x66666666, 0x66666666, 0x66666666,
	0x66666666, 0x66666666, 0x66666666, 0x66666666
	};

/* Bz = 1 */
static const uint32 _ed25519_fast_bz[8] = {
	1, 0, 0, 0, 0, 0, 0, 0
	};

/* Bt = (Bx * By) % p
*       46827403850823179245072216630277197565144205554125654976674165829533817101731 */
static const uint32 _ed25519_fast_bt[8] = {
	0xa5b7dda3, 0x6dde8ab3, 0x775152f5, 0x20f09f80,
	0x64abe37d, 0x66ea4e8e, 0xd78b7665, 0x67875f0f
	};

#undef ed25519_point
struct ed25519_point {
	uint32 x[8];
	uint32 y[8];
	uint32 z[8];
	uint32 t[8];
};

#undef ED25519_FAST_BASE
#define ED25519_FAST_BASE _ed25519_fast_base
static const struct ed25519_point _ed25519_fast_base = {
	{
		0x8f25d51a, 0xc9562d60, 0x9525a7b2, 0x692cc760,
		0xfdd6dc5c, 0xc0a4e231, 0xcd6e53fe, 0x216936d3
	},
	{
		0x66666658, 0x66666666, 0x66666666, 0x66666666,
		0x66666666, 0x66666666, 0x66666666, 0x66666666
	},
	{
		1, 0, 0, 0, 0, 0, 0, 0
	},
	{
		0xa5b7dda3, 0x6dde8ab3, 0x775152f5, 0x20f09f80,
		0x64abe37d, 0x66ea4e8e, 0xd78b7665, 0x67875f0f
	}
	};

#undef ED25519_LEN
#define ED25519_LEN 32
#undef ED25519_SIGN_LEN
#define ED25519_SIGN_LEN 64
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* ed25519_fast.c */
extern void FSYMBOL(ed25519_fast_point_add)(const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2,
		struct ed25519_point *xyz3);
extern void FSYMBOL(ed25519_fast_scalar_mul)(const uint32 k[8],
		const struct ed25519_point *xyz1,
		struct ed25519_point *xyz2);
extern int32 FSYMBOL(ed25519_fast_point_equal)(const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2);
extern void FSYMBOL(ed25519_fast_point_recover_x)(const uint32 y[8], uint32 sign,
		uint32 r[8]);
extern void FSYMBOL(ed25519_fast_point_compress)(const struct ed25519_point *xyz1,
		uint32 r[8]);
extern void FSYMBOL(ed25519_fast_point_decompress)(const uint32 k[8],
		struct ed25519_point *xyz1);
extern void FSYMBOL(ed25519_fast_clamp_key)(uint32 k[8]);

/* ed25519_fast_sign.c */
extern void FSYMBOL(ed25519_fast_private_key)(const uint8 *key, uint32 pri[8],
		uint32 ran[8]);
extern void FSYMBOL(ed25519_fast_public_key)(const uint8 *pri, uint8 *pub);
extern void FSYMBOL(ed25519_fast_sign)(const uint8 *pri, const uint8 *mesg,
		uint64 len, uint8 *sign);
extern int32 FSYMBOL(ed25519_fast_verify)(const uint8 *pub, const uint8 *sign,
		const uint8 *mesg, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
