/* x448_fast.h - curve448 elliptic-curve crypto (ecc) definitions */

#ifndef LIBF_AL_X448_FAST_H
#define LIBF_AL_X448_FAST_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: x448_fast */
#undef X448_FAST_P
#define X448_FAST_P _x448_fast_p
#undef X448_FAST_B
#define X448_FAST_B _x448_fast_b

/* P = 2**448 - 2**224 - 1 */
static const uint32 _x448_fast_p[14] = {
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffe,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff,
	};

/* B = 5 */
static const uint32 _x448_fast_b[14] = {
	5, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0
	};

#undef X448_LEN
#define X448_LEN 56
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* x448_fast.c */
extern void FSYMBOL(x448_fast_scalar_mul)(const uint32 k[14], const uint32 b[14],
		uint32 r[14]);
extern void FSYMBOL(x448_fast_clamp_key)(uint32 k[14]);
extern void FSYMBOL(x448_fast_private_key)(uint8 *pri);
extern void FSYMBOL(x448_fast_public_key)(const uint8 *pri, uint8 *pub);
extern void FSYMBOL(x448_fast_shared_key)(const uint8 *pri, const uint8 *pub,
		uint8 *key);

#ifdef __cplusplus
}
#endif


#endif
