/* bn_4097.h - big number calculation (4097-bit) definitions */

#ifndef LIBF_AL_BN_4097_H
#define LIBF_AL_BN_4097_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bn_4097 */
#undef BN_4097_MAX
#define BN_4097_MAX (128 + 1 + 1)
#undef BN_4097_SIG
#define BN_4097_SIG (128 + 1)
#undef BN_4097_SIZE
#define BN_4097_SIZE (128 + 1)
#undef BN_4096_SIZE
#define BN_4096_SIZE 128
#undef BN_4097_BITS
#define BN_4097_BITS (BN_4097_SIZE * 32)

#undef bn_4097_t
typedef uint32 bn_4097_t[BN_4097_MAX];

typedef struct bn_i4097_redc_ctx {
	bn_4097_t R;
	bn_4097_t Rs;
	bn_4097_t np;
	bn_4097_t mask;
	int32 k;
} bn_i4097_redc_t;
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_4097_base.c */
extern void FSYMBOL(bn_4097_init)(bn_4097_t t);
extern void FSYMBOL(bn_4097_move)(bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_4097_u8copy)(bn_4097_t a, const uint8 *b, uint32 len);
extern void FSYMBOL(bn_4097_swap)(bn_4097_t a, bn_4097_t b);
extern void FSYMBOL(bn_4097_set1)(bn_4097_t t, uint32 n);
extern int32 FSYMBOL(bn_4097_bits)(const bn_4097_t n);
extern void FSYMBOL(bn_4097_lsh)(bn_4097_t t, uint32 n);
extern void FSYMBOL(bn_4097_rsh)(bn_4097_t t, uint32 n);
extern void FSYMBOL(bn_4097_and)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_4097_or)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_4097_xor)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_4097_not)(bn_4097_t r,
		const bn_4097_t a);
extern void FSYMBOL(bn_4097_strtonum)(bn_4097_t t,
		const char *s, char **e, int32 b);

/* bn_4097_gcd.c */
extern void FSYMBOL(bn_i4097_gcd)(bn_4097_t r, bn_4097_t x, bn_4097_t y,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_i4097_gcd_ext)(bn_4097_t r, bn_4097_t x, bn_4097_t y,
		const bn_4097_t a, const bn_4097_t b);
extern int32 FSYMBOL(bn_i4097_inv)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);

/* bn_4097_int.c */
extern int32 FSYMBOL(bn_i4097_cmp)(const bn_4097_t a, const bn_4097_t b);
extern int32 FSYMBOL(bn_i4097_cmp1)(const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_i4097_add)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_i4097_sub)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_i4097_mul)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_i4097_div)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_i4097_divmod)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_i4097_mul1)(bn_4097_t r,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_i4097_add1)(bn_4097_t r,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_i4097_sub1)(bn_4097_t r,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_i4097_div1)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_i4097_divmod1)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_i4097_mod_pow)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b,
		const bn_4097_t m);

/* bn_4097_redc.c */
extern int32 FSYMBOL(bn_i4097_redc_init)(bn_i4097_redc_t *ctx,
		const bn_4097_t n);
extern void FSYMBOL(bn_i4097_redc_mod)(bn_i4097_redc_t *ctx, bn_4097_t r,
		const bn_4097_t a, const bn_4097_t n);
extern void FSYMBOL(bn_i4097_redc_moto)(bn_i4097_redc_t *ctx, bn_4097_t r,
		const bn_4097_t a, const bn_4097_t n);
extern int32 FSYMBOL(bn_i4097_redc_pow)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b,
		const bn_4097_t m);

/* bn_4097_uint.c */
extern int32 FSYMBOL(bn_u4097_cmp)(const bn_4097_t a, const bn_4097_t b);
extern int32 FSYMBOL(bn_u4097_cmp1)(const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_u4097_add)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_u4097_sub)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_u4097_mul)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_u4097_div)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, const bn_4097_t b);
extern void FSYMBOL(bn_u4097_add1)(bn_4097_t r,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_u4097_sub1)(bn_4097_t r,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_u4097_mul1)(bn_4097_t r,
		const bn_4097_t a, uint32 b);
extern void FSYMBOL(bn_u4097_div1)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, uint32 b);

#ifdef __cplusplus
}
#endif


#endif
