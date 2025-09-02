/* bn_8193.h - big number calculation (8193-bit) definitions */

#ifndef LIBF_AL_BN_8193_H
#define LIBF_AL_BN_8193_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bn_8193 */
#undef BN_8193_MAX
#define BN_8193_MAX (256 + 1 + 1)
#undef BN_8193_SIG
#define BN_8193_SIG (256 + 1)
#undef BN_8193_SIZE
#define BN_8193_SIZE (256 + 1)
#undef BN_8192_SIZE
#define BN_8192_SIZE 256
#undef BN_8193_BITS
#define BN_8193_BITS (BN_8193_SIZE * 32)

#undef bn_8193_t
typedef uint32 bn_8193_t[BN_8193_MAX];

typedef struct bn_i8193_redc_ctx {
	bn_8193_t R;
	bn_8193_t Rs;
	bn_8193_t np;
	bn_8193_t mask;
	int32 k;
} bn_i8193_redc_t;
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_8193_base.c */
extern void FSYMBOL(bn_8193_init)(bn_8193_t t);
extern void FSYMBOL(bn_8193_move)(bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_8193_u8copy)(bn_8193_t a, const uint8 *b, uint32 len);
extern void FSYMBOL(bn_8193_swap)(bn_8193_t a, bn_8193_t b);
extern void FSYMBOL(bn_8193_set1)(bn_8193_t t, uint32 n);
extern int32 FSYMBOL(bn_8193_bits)(const bn_8193_t n);
extern void FSYMBOL(bn_8193_lsh)(bn_8193_t t, uint32 n);
extern void FSYMBOL(bn_8193_rsh)(bn_8193_t t, uint32 n);
extern void FSYMBOL(bn_8193_and)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_8193_or)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_8193_xor)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_8193_not)(bn_8193_t r,
		const bn_8193_t a);
extern void FSYMBOL(bn_8193_strtonum)(bn_8193_t t,
		const char *s, char **e, int32 b);

/* bn_8193_gcd.c */
extern void FSYMBOL(bn_i8193_gcd)(bn_8193_t r, bn_8193_t x, bn_8193_t y,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_i8193_gcd_ext)(bn_8193_t r, bn_8193_t x, bn_8193_t y,
		const bn_8193_t a, const bn_8193_t b);
extern int32 FSYMBOL(bn_i8193_inv)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);

/* bn_8193_int.c */
extern int32 FSYMBOL(bn_i8193_cmp)(const bn_8193_t a, const bn_8193_t b);
extern int32 FSYMBOL(bn_i8193_cmp1)(const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_i8193_add)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_i8193_sub)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_i8193_mul)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_i8193_div)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_i8193_divmod)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_i8193_mul1)(bn_8193_t r,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_i8193_add1)(bn_8193_t r,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_i8193_sub1)(bn_8193_t r,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_i8193_div1)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_i8193_divmod1)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_i8193_mod_pow)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b,
		const bn_8193_t m);

/* bn_8193_redc.c */
extern int32 FSYMBOL(bn_i8193_redc_init)(bn_i8193_redc_t *ctx,
		const bn_8193_t n);
extern void FSYMBOL(bn_i8193_redc_mod)(bn_i8193_redc_t *ctx, bn_8193_t r,
		const bn_8193_t a, const bn_8193_t n);
extern void FSYMBOL(bn_i8193_redc_moto)(bn_i8193_redc_t *ctx, bn_8193_t r,
		const bn_8193_t a, const bn_8193_t n);
extern int32 FSYMBOL(bn_i8193_redc_pow)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b,
		const bn_8193_t m);

/* bn_8193_uint.c */
extern int32 FSYMBOL(bn_u8193_cmp)(const bn_8193_t a, const bn_8193_t b);
extern int32 FSYMBOL(bn_u8193_cmp1)(const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_u8193_add)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_u8193_sub)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_u8193_mul)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_u8193_div)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, const bn_8193_t b);
extern void FSYMBOL(bn_u8193_add1)(bn_8193_t r,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_u8193_sub1)(bn_8193_t r,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_u8193_mul1)(bn_8193_t r,
		const bn_8193_t a, uint32 b);
extern void FSYMBOL(bn_u8193_div1)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, uint32 b);

#ifdef __cplusplus
}
#endif


#endif
