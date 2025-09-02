/* bn_2049.h - big number calculation (2049-bit) definitions */

#ifndef LIBF_AL_BN_2049_H
#define LIBF_AL_BN_2049_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bn_2049 */
#undef BN_2049_MAX
#define BN_2049_MAX (64 + 1 + 1)
#undef BN_2049_SIG
#define BN_2049_SIG (64 + 1)
#undef BN_2049_SIZE
#define BN_2049_SIZE (64 + 1)
#undef BN_2048_SIZE
#define BN_2048_SIZE 64
#undef BN_2049_BITS
#define BN_2049_BITS (BN_2049_SIZE * 32)

#undef bn_2049_t
typedef uint32 bn_2049_t[BN_2049_MAX];

typedef struct bn_i2049_redc_ctx {
	bn_2049_t R;
	bn_2049_t Rs;
	bn_2049_t np;
	bn_2049_t mask;
	int32 k;
} bn_i2049_redc_t;
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_2049_base.c */
extern void FSYMBOL(bn_2049_init)(bn_2049_t t);
extern void FSYMBOL(bn_2049_move)(bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_2049_u8copy)(bn_2049_t a, const uint8 *b, uint32 len);
extern void FSYMBOL(bn_2049_swap)(bn_2049_t a, bn_2049_t b);
extern void FSYMBOL(bn_2049_set1)(bn_2049_t t, uint32 n);
extern int32 FSYMBOL(bn_2049_bits)(const bn_2049_t n);
extern void FSYMBOL(bn_2049_lsh)(bn_2049_t t, uint32 n);
extern void FSYMBOL(bn_2049_rsh)(bn_2049_t t, uint32 n);
extern void FSYMBOL(bn_2049_and)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_2049_or)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_2049_xor)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_2049_not)(bn_2049_t r,
		const bn_2049_t a);
extern void FSYMBOL(bn_2049_strtonum)(bn_2049_t t,
		const char *s, char **e, int32 b);

/* bn_2049_gcd.c */
extern void FSYMBOL(bn_i2049_gcd)(bn_2049_t r, bn_2049_t x, bn_2049_t y,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_i2049_gcd_ext)(bn_2049_t r, bn_2049_t x, bn_2049_t y,
		const bn_2049_t a, const bn_2049_t b);
extern int32 FSYMBOL(bn_i2049_inv)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);

/* bn_2049_int.c */
extern int32 FSYMBOL(bn_i2049_cmp)(const bn_2049_t a, const bn_2049_t b);
extern int32 FSYMBOL(bn_i2049_cmp1)(const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_i2049_add)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_i2049_sub)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_i2049_mul)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_i2049_div)(bn_2049_t quo, bn_2049_t rem,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_i2049_divmod)(bn_2049_t quo, bn_2049_t rem,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_i2049_mul1)(bn_2049_t r,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_i2049_add1)(bn_2049_t r,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_i2049_sub1)(bn_2049_t r,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_i2049_div1)(bn_2049_t quo, bn_2049_t rem,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_i2049_divmod1)(bn_2049_t quo, bn_2049_t rem,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_i2049_mod_pow)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b,
		const bn_2049_t m);

/* bn_2049_redc.c */
extern int32 FSYMBOL(bn_i2049_redc_init)(bn_i2049_redc_t *ctx,
		const bn_2049_t n);
extern void FSYMBOL(bn_i2049_redc_mod)(bn_i2049_redc_t *ctx, bn_2049_t r,
		const bn_2049_t a, const bn_2049_t n);
extern void FSYMBOL(bn_i2049_redc_moto)(bn_i2049_redc_t *ctx, bn_2049_t r,
		const bn_2049_t a, const bn_2049_t n);
extern int32 FSYMBOL(bn_i2049_redc_pow)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b,
		const bn_2049_t m);

/* bn_2049_uint.c */
extern int32 FSYMBOL(bn_u2049_cmp)(const bn_2049_t a, const bn_2049_t b);
extern int32 FSYMBOL(bn_u2049_cmp1)(const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_u2049_add)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_u2049_sub)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_u2049_mul)(bn_2049_t r,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_u2049_div)(bn_2049_t quo, bn_2049_t rem,
		const bn_2049_t a, const bn_2049_t b);
extern void FSYMBOL(bn_u2049_add1)(bn_2049_t r,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_u2049_sub1)(bn_2049_t r,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_u2049_mul1)(bn_2049_t r,
		const bn_2049_t a, uint32 b);
extern void FSYMBOL(bn_u2049_div1)(bn_2049_t quo, bn_2049_t rem,
		const bn_2049_t a, uint32 b);

#ifdef __cplusplus
}
#endif


#endif
