/* bn_3073.h - big number calculation (3073-bit) definitions */

#ifndef LIBF_AL_BN_3073_H
#define LIBF_AL_BN_3073_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bn_3073 */
#undef BN_3073_MAX
#define BN_3073_MAX (96 + 1 + 1)
#undef BN_3073_SIG
#define BN_3073_SIG (96 + 1)
#undef BN_3073_SIZE
#define BN_3073_SIZE (96 + 1)
#undef BN_3072_SIZE
#define BN_3072_SIZE 96
#undef BN_3073_BITS
#define BN_3073_BITS (BN_3073_SIZE * 32)

#undef bn_3073_t
typedef uint32 bn_3073_t[BN_3073_MAX];

typedef struct bn_i3073_redc_ctx {
	bn_3073_t R;
	bn_3073_t Rs;
	bn_3073_t np;
	bn_3073_t mask;
	int32 k;
} bn_i3073_redc_t;
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_3073_base.c */
extern void FSYMBOL(bn_3073_init)(bn_3073_t t);
extern void FSYMBOL(bn_3073_move)(bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_3073_u8copy)(bn_3073_t a, const uint8 *b, uint32 len);
extern void FSYMBOL(bn_3073_swap)(bn_3073_t a, bn_3073_t b);
extern void FSYMBOL(bn_3073_set1)(bn_3073_t t, uint32 n);
extern int32 FSYMBOL(bn_3073_bits)(const bn_3073_t n);
extern void FSYMBOL(bn_3073_lsh)(bn_3073_t t, uint32 n);
extern void FSYMBOL(bn_3073_rsh)(bn_3073_t t, uint32 n);
extern void FSYMBOL(bn_3073_and)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_3073_or)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_3073_xor)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_3073_not)(bn_3073_t r,
		const bn_3073_t a);
extern void FSYMBOL(bn_3073_strtonum)(bn_3073_t t,
		const char *s, char **e, int32 b);

/* bn_3073_gcd.c */
extern void FSYMBOL(bn_i3073_gcd)(bn_3073_t r, bn_3073_t x, bn_3073_t y,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_i3073_gcd_ext)(bn_3073_t r, bn_3073_t x, bn_3073_t y,
		const bn_3073_t a, const bn_3073_t b);
extern int32 FSYMBOL(bn_i3073_inv)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);

/* bn_3073_int.c */
extern int32 FSYMBOL(bn_i3073_cmp)(const bn_3073_t a, const bn_3073_t b);
extern int32 FSYMBOL(bn_i3073_cmp1)(const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_i3073_add)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_i3073_sub)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_i3073_mul)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_i3073_div)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_i3073_divmod)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_i3073_mul1)(bn_3073_t r,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_i3073_add1)(bn_3073_t r,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_i3073_sub1)(bn_3073_t r,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_i3073_div1)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_i3073_divmod1)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_i3073_mod_pow)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b,
		const bn_3073_t m);

/* bn_3073_redc.c */
extern int32 FSYMBOL(bn_i3073_redc_init)(bn_i3073_redc_t *ctx,
		const bn_3073_t n);
extern void FSYMBOL(bn_i3073_redc_mod)(bn_i3073_redc_t *ctx, bn_3073_t r,
		const bn_3073_t a, const bn_3073_t n);
extern void FSYMBOL(bn_i3073_redc_moto)(bn_i3073_redc_t *ctx, bn_3073_t r,
		const bn_3073_t a, const bn_3073_t n);
extern int32 FSYMBOL(bn_i3073_redc_pow)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b,
		const bn_3073_t m);

/* bn_3073_uint.c */
extern int32 FSYMBOL(bn_u3073_cmp)(const bn_3073_t a, const bn_3073_t b);
extern int32 FSYMBOL(bn_u3073_cmp1)(const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_u3073_add)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_u3073_sub)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_u3073_mul)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_u3073_div)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, const bn_3073_t b);
extern void FSYMBOL(bn_u3073_add1)(bn_3073_t r,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_u3073_sub1)(bn_3073_t r,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_u3073_mul1)(bn_3073_t r,
		const bn_3073_t a, uint32 b);
extern void FSYMBOL(bn_u3073_div1)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, uint32 b);

#ifdef __cplusplus
}
#endif


#endif
