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

#undef X25519_LEN
#define X25519_LEN 32
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* x25519_fast.c */
extern void FSYMBOL(x25519_fast_scalar_mul)(const uint32 k[8], const uint32 b[8],
		uint32 r[8]);
extern void FSYMBOL(x25519_fast_clamp_key)(uint32 k[8]);
extern void FSYMBOL(x25519_fast_base_mask)(uint32 b[8]);
extern void FSYMBOL(x25519_fast_private_key)(uint8 *pri);
extern void FSYMBOL(x25519_fast_public_key)(const uint8 *pri, uint8 *pub);
extern void FSYMBOL(x25519_fast_shared_key)(const uint8 *pri, const uint8 *pub,
		uint8 *key);

#ifdef __cplusplus
}
#endif


#endif
