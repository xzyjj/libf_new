/* bn_1025.h - big number calculation (1025-bit) definitions */

#ifndef LIBF_AL_BN_1025_H
#define LIBF_AL_BN_1025_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bn_1025 */
#undef BN_1025_MAX
#define BN_1025_MAX (32 + 1 + 1)
#undef BN_1025_SIG
#define BN_1025_SIG (32 + 1)
#undef BN_1025_SIZE
#define BN_1025_SIZE (32 + 1)
#undef BN_1024_SIZE
#define BN_1024_SIZE 32
#undef BN_1025_BITS
#define BN_1025_BITS (BN_1025_SIZE * 32)

#undef bn_1025_t
typedef uint32 bn_1025_t[BN_1025_MAX];

typedef struct bn_i1025_redc_ctx {
	bn_1025_t R;
	bn_1025_t Rs;
	bn_1025_t np;
	bn_1025_t mask;
	int32 k;
} bn_i1025_redc_t;
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_1025_base.c */
extern void FSYMBOL(bn_1025_init)(bn_1025_t t);
extern void FSYMBOL(bn_1025_move)(bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_1025_u8copy)(bn_1025_t a, const uint8 *b, uint32 len);
extern void FSYMBOL(bn_1025_swap)(bn_1025_t a, bn_1025_t b);
extern void FSYMBOL(bn_1025_set1)(bn_1025_t t, uint32 n);
extern int32 FSYMBOL(bn_1025_bits)(const bn_1025_t n);
extern void FSYMBOL(bn_1025_lsh)(bn_1025_t t, uint32 n);
extern void FSYMBOL(bn_1025_rsh)(bn_1025_t t, uint32 n);
extern void FSYMBOL(bn_1025_and)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_1025_or)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_1025_xor)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_1025_not)(bn_1025_t r,
		const bn_1025_t a);
extern void FSYMBOL(bn_1025_strtonum)(bn_1025_t t,
		const char *s, char **e, int32 b);

/* bn_1025_gcd.c */
extern void FSYMBOL(bn_i1025_gcd)(bn_1025_t r, bn_1025_t x, bn_1025_t y,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_i1025_gcd_ext)(bn_1025_t r, bn_1025_t x, bn_1025_t y,
		const bn_1025_t a, const bn_1025_t b);
extern int32 FSYMBOL(bn_i1025_inv)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);

/* bn_1025_int.c */
extern int32 FSYMBOL(bn_i1025_cmp)(const bn_1025_t a, const bn_1025_t b);
extern int32 FSYMBOL(bn_i1025_cmp1)(const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_i1025_add)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_i1025_sub)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_i1025_mul)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_i1025_div)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_i1025_divmod)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_i1025_mul1)(bn_1025_t r,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_i1025_add1)(bn_1025_t r,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_i1025_sub1)(bn_1025_t r,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_i1025_div1)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_i1025_divmod1)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_i1025_mod_pow)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b,
		const bn_1025_t m);

/* bn_1025_redc.c */
extern int32 FSYMBOL(bn_i1025_redc_init)(bn_i1025_redc_t *ctx,
		const bn_1025_t n);
extern void FSYMBOL(bn_i1025_redc_mod)(bn_i1025_redc_t *ctx, bn_1025_t r,
		const bn_1025_t a, const bn_1025_t n);
extern void FSYMBOL(bn_i1025_redc_moto)(bn_i1025_redc_t *ctx, bn_1025_t r,
		const bn_1025_t a, const bn_1025_t n);
extern int32 FSYMBOL(bn_i1025_redc_pow)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b,
		const bn_1025_t m);

/* bn_1025_uint.c */
extern int32 FSYMBOL(bn_u1025_cmp)(const bn_1025_t a, const bn_1025_t b);
extern int32 FSYMBOL(bn_u1025_cmp1)(const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_u1025_add)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_u1025_sub)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_u1025_mul)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_u1025_div)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, const bn_1025_t b);
extern void FSYMBOL(bn_u1025_add1)(bn_1025_t r,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_u1025_sub1)(bn_1025_t r,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_u1025_mul1)(bn_1025_t r,
		const bn_1025_t a, uint32 b);
extern void FSYMBOL(bn_u1025_div1)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, uint32 b);

#ifdef __cplusplus
}
#endif


#endif
