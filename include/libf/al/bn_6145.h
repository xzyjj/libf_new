/* bn_6145.h - big number calculation (6145-bit) definitions */

#ifndef LIBF_AL_BN_6145_H
#define LIBF_AL_BN_6145_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: bn_6145 */
#undef BN_6145_MAX
#define BN_6145_MAX (192 + 1 + 1)
#undef BN_6145_SIG
#define BN_6145_SIG (192 + 1)
#undef BN_6145_SIZE
#define BN_6145_SIZE (192 + 1)
#undef BN_6144_SIZE
#define BN_6144_SIZE 192
#undef BN_6145_BITS
#define BN_6145_BITS (BN_6145_SIZE * 32)

#undef bn_6145_t
typedef uint32 bn_6145_t[BN_6145_MAX];

typedef struct bn_i6145_redc_ctx {
	bn_6145_t R;
	bn_6145_t Rs;
	bn_6145_t np;
	bn_6145_t mask;
	int32 k;
} bn_i6145_redc_t;
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_6145_base.c */
extern void FSYMBOL(bn_6145_init)(bn_6145_t t);
extern void FSYMBOL(bn_6145_move)(bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_6145_u8copy)(bn_6145_t a, const uint8 *b, uint32 len);
extern void FSYMBOL(bn_6145_swap)(bn_6145_t a, bn_6145_t b);
extern void FSYMBOL(bn_6145_set1)(bn_6145_t t, uint32 n);
extern int32 FSYMBOL(bn_6145_bits)(const bn_6145_t n);
extern void FSYMBOL(bn_6145_lsh)(bn_6145_t t, uint32 n);
extern void FSYMBOL(bn_6145_rsh)(bn_6145_t t, uint32 n);
extern void FSYMBOL(bn_6145_and)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_6145_or)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_6145_xor)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_6145_not)(bn_6145_t r,
		const bn_6145_t a);
extern void FSYMBOL(bn_6145_strtonum)(bn_6145_t t,
		const char *s, char **e, int32 b);

/* bn_6145_gcd.c */
extern void FSYMBOL(bn_i6145_gcd)(bn_6145_t r, bn_6145_t x, bn_6145_t y,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_i6145_gcd_ext)(bn_6145_t r, bn_6145_t x, bn_6145_t y,
		const bn_6145_t a, const bn_6145_t b);
extern int32 FSYMBOL(bn_i6145_inv)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);

/* bn_6145_int.c */
extern int32 FSYMBOL(bn_i6145_cmp)(const bn_6145_t a, const bn_6145_t b);
extern int32 FSYMBOL(bn_i6145_cmp1)(const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_i6145_add)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_i6145_sub)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_i6145_mul)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_i6145_div)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_i6145_divmod)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_i6145_mul1)(bn_6145_t r,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_i6145_add1)(bn_6145_t r,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_i6145_sub1)(bn_6145_t r,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_i6145_div1)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_i6145_divmod1)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_i6145_mod_pow)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b,
		const bn_6145_t m);

/* bn_6145_redc.c */
extern int32 FSYMBOL(bn_i6145_redc_init)(bn_i6145_redc_t *ctx,
		const bn_6145_t n);
extern void FSYMBOL(bn_i6145_redc_mod)(bn_i6145_redc_t *ctx, bn_6145_t r,
		const bn_6145_t a, const bn_6145_t n);
extern void FSYMBOL(bn_i6145_redc_moto)(bn_i6145_redc_t *ctx, bn_6145_t r,
		const bn_6145_t a, const bn_6145_t n);
extern int32 FSYMBOL(bn_i6145_redc_pow)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b,
		const bn_6145_t m);

/* bn_6145_uint.c */
extern int32 FSYMBOL(bn_u6145_cmp)(const bn_6145_t a, const bn_6145_t b);
extern int32 FSYMBOL(bn_u6145_cmp1)(const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_u6145_add)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_u6145_sub)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_u6145_mul)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_u6145_div)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, const bn_6145_t b);
extern void FSYMBOL(bn_u6145_add1)(bn_6145_t r,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_u6145_sub1)(bn_6145_t r,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_u6145_mul1)(bn_6145_t r,
		const bn_6145_t a, uint32 b);
extern void FSYMBOL(bn_u6145_div1)(bn_6145_t quo, bn_6145_t rem,
		const bn_6145_t a, uint32 b);

#ifdef __cplusplus
}
#endif


#endif
