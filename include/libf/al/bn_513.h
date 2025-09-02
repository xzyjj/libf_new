/* bn_513.h - big number calculation (513-bit) definitions */

#ifndef LIBF_AL_BN_513_H
#define LIBF_AL_BN_513_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bn_513 */
#undef BN_513_MAX
#define BN_513_MAX (16 + 1 + 1)
#undef BN_513_SIG
#define BN_513_SIG (16 + 1)
#undef BN_513_SIZE
#define BN_513_SIZE (16 + 1)
#undef BN_512_SIZE
#define BN_512_SIZE 16
#undef BN_513_BITS
#define BN_513_BITS (BN_513_SIZE * 32)

#undef bn_513_t
typedef uint32 bn_513_t[BN_513_MAX];

typedef struct bn_i513_redc_ctx {
	bn_513_t R;
	bn_513_t Rs;
	bn_513_t np;
	bn_513_t mask;
	int32 k;
} bn_i513_redc_t;
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_513_base.c */
extern void FSYMBOL(bn_513_init)(bn_513_t t);
extern void FSYMBOL(bn_513_move)(bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_513_u8copy)(bn_513_t a, const uint8 *b, uint32 len);
extern void FSYMBOL(bn_513_swap)(bn_513_t a, bn_513_t b);
extern void FSYMBOL(bn_513_set1)(bn_513_t t, uint32 n);
extern int32 FSYMBOL(bn_513_bits)(const bn_513_t n);
extern void FSYMBOL(bn_513_lsh)(bn_513_t t, uint32 n);
extern void FSYMBOL(bn_513_rsh)(bn_513_t t, uint32 n);
extern void FSYMBOL(bn_513_and)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_513_or)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_513_xor)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_513_not)(bn_513_t r,
		const bn_513_t a);
extern void FSYMBOL(bn_513_strtonum)(bn_513_t t,
		const char *s, char **e, int32 b);

/* bn_513_gcd.c */
extern void FSYMBOL(bn_i513_gcd)(bn_513_t r, bn_513_t x, bn_513_t y,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_i513_gcd_ext)(bn_513_t r, bn_513_t x, bn_513_t y,
		const bn_513_t a, const bn_513_t b);
extern int32 FSYMBOL(bn_i513_inv)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);

/* bn_513_int.c */
extern int32 FSYMBOL(bn_i513_cmp)(const bn_513_t a, const bn_513_t b);
extern int32 FSYMBOL(bn_i513_cmp1)(const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_i513_add)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_i513_sub)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_i513_mul)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_i513_div)(bn_513_t quo, bn_513_t rem,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_i513_divmod)(bn_513_t quo, bn_513_t rem,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_i513_mul1)(bn_513_t r,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_i513_add1)(bn_513_t r,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_i513_sub1)(bn_513_t r,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_i513_div1)(bn_513_t quo, bn_513_t rem,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_i513_divmod1)(bn_513_t quo, bn_513_t rem,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_i513_mod_pow)(bn_513_t r,
		const bn_513_t a, const bn_513_t b,
		const bn_513_t m);

/* bn_513_redc.c */
extern int32 FSYMBOL(bn_i513_redc_init)(bn_i513_redc_t *ctx,
		const bn_513_t n);
extern void FSYMBOL(bn_i513_redc_mod)(bn_i513_redc_t *ctx, bn_513_t r,
		const bn_513_t a, const bn_513_t n);
extern void FSYMBOL(bn_i513_redc_moto)(bn_i513_redc_t *ctx, bn_513_t r,
		const bn_513_t a, const bn_513_t n);
extern int32 FSYMBOL(bn_i513_redc_pow)(bn_513_t r,
		const bn_513_t a, const bn_513_t b,
		const bn_513_t m);

/* bn_513_uint.c */
extern int32 FSYMBOL(bn_u513_cmp)(const bn_513_t a, const bn_513_t b);
extern int32 FSYMBOL(bn_u513_cmp1)(const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_u513_add)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_u513_sub)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_u513_mul)(bn_513_t r,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_u513_div)(bn_513_t quo, bn_513_t rem,
		const bn_513_t a, const bn_513_t b);
extern void FSYMBOL(bn_u513_add1)(bn_513_t r,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_u513_sub1)(bn_513_t r,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_u513_mul1)(bn_513_t r,
		const bn_513_t a, uint32 b);
extern void FSYMBOL(bn_u513_div1)(bn_513_t quo, bn_513_t rem,
		const bn_513_t a, uint32 b);

#ifdef __cplusplus
}
#endif


#endif
