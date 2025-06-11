/* x25519_fast.h - curve25519 elliptic-curve crypto (ecc) definitions */

#ifndef LIBF_AL_X25519_FAST_H
#define LIBF_AL_X25519_FAST_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: x25519_fast */
#undef X25519_FAST_P
#define X25519_FAST_P _x25519_fast_p
#undef X25519_FAST_B
#define X25519_FAST_B _x25519_fast_b

/* P = 2**255 - 19 */
static const uint32 _x25519_fast_p[8] = {
	0xffffffed, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0x7fffffff
	};

/* B = 9 */
static const uint32 _x25519_fast_b[8] = {
	9, 0, 0, 0, 0, 0, 0, 0
	};

#undef X25519_FAST_ECDH_NEW
#define X25519_FAST_ECDH_NEW(x) struct x25519_fast_ecdh_ctx x

#undef x25519_fast_ecdh_ctx
struct x25519_fast_ecdh_ctx {
	uint32 b[8];
	uint32 pri[8], pub[8];
	uint32 key[8];
};

#undef X25519_FAST_LEN
#define X25519_FAST_LEN 32

#undef X25519_FAST_PRI
#define X25519_FAST_PRI(x, n) (((uint8 *)(x)->pri)[n])
#undef X25519_FAST_PUB
#define X25519_FAST_PUB(x, n) (((uint8 *)(x)->pub)[n])

#undef X25519_FAST_KEY
#define X25519_FAST_KEY(x, n) (((uint8 *)(x)->key)[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* x25519_fast.c */
extern void FSYMBOL(x25519_fast_scalar_mul)(const uint32 k[8], const uint32 b[8],
		uint32 r[8]);
extern void FSYMBOL(x25519_fast_clamp_key)(uint32 k[8]);
extern void FSYMBOL(x25519_fast_base_mask)(uint32 b[8]);

/* x25519_fast_ecdh.c */
extern void FSYMBOL(x25519_fast_ecdh_init)(struct x25519_fast_ecdh_ctx *ctx);
extern void FSYMBOL(x25519_fast_ecdh_private)(struct x25519_fast_ecdh_ctx *ctx,
		const uint8 *key);
extern void FSYMBOL(x25519_fast_ecdh_public)(struct x25519_fast_ecdh_ctx *ctx);
extern void FSYMBOL(x25519_fast_ecdh_exchange)(struct x25519_fast_ecdh_ctx *ctx,
		const uint8 *key);

#ifdef __cplusplus
}
#endif


#endif
