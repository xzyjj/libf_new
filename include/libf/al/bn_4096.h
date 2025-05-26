/* bn_4096.h - big number calculation (4096bit) definitions */

#ifndef LIBF_AL_BN_4096_H
#define LIBF_AL_BN_4096_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: bn_4096 */
#undef bn_uint4096_t
typedef uint32 bn_uint4096_t[130];
#undef bn_int4096_t
typedef uint32 bn_int4096_t[130];

#undef BN_4096_SIZE
#define BN_4096_SIZE 128
#undef BN_4096_SIG
#define BN_4096_SIG 129
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_i4096.c */
extern void FSYMBOL(bn_int4096_zero)(bn_int4096_t t);
extern void FSYMBOL(bn_int4096_move)(bn_int4096_t t, const bn_int4096_t s);
extern void FSYMBOL(bn_int4096_moveu)(bn_uint4096_t t, const bn_int4096_t s);
extern void FSYMBOL(bn_int4096_movei)(bn_int4096_t t, const bn_uint4096_t s);
extern int32 FSYMBOL(bn_int4096_cmp)(const bn_int4096_t a, const bn_int4096_t b);
extern int32 FSYMBOL(bn_int4097_cmp)(const bn_int4096_t a, const bn_int4096_t b);
extern int32 FSYMBOL(bn_int4096_cmp_1)(const bn_int4096_t a, uint32 b);
extern void FSYMBOL(bn_int4096_lsh)(bn_int4096_t n);
extern void FSYMBOL(bn_int4097_lsh)(bn_int4096_t n);
extern void FSYMBOL(bn_int4096_rsh)(bn_int4096_t n);
extern void FSYMBOL(bn_int4097_rsh)(bn_int4096_t n);
extern void FSYMBOL(bn_int4096_and)(bn_int4096_t r, bn_int4096_t a, bn_int4096_t b);
extern void FSYMBOL(bn_int4096_or)(bn_int4096_t r, bn_int4096_t a, bn_int4096_t b);
extern void FSYMBOL(bn_int4096_xor)(bn_int4096_t r, bn_int4096_t a, bn_int4096_t b);
extern void FSYMBOL(bn_int4096_not)(bn_int4096_t r, bn_int4096_t n);
extern void FSYMBOL(bn_int4096_add)(bn_int4096_t r, const bn_int4096_t a,
		const bn_int4096_t b);
extern void FSYMBOL(bn_int4096_sub)(bn_int4096_t r, const bn_int4096_t a,
		const bn_int4096_t b);
extern void FSYMBOL(bn_int4096_mul)(bn_int4096_t r, const bn_int4096_t a,
		const bn_int4096_t b);
extern void FSYMBOL(bn_int4096_div)(bn_int4096_t quo, bn_int4096_t rem,
		const bn_int4096_t a, const bn_int4096_t b);
extern void FSYMBOL(bn_int4096_divmod)(bn_int4096_t quo, bn_int4096_t rem,
		const bn_int4096_t a, const bn_int4096_t b);
extern void FSYMBOL(bn_int4096_strtonum)(bn_int4096_t t, const char *s, char **e,
		int32 b);
extern void FSYMBOL(bn_int4096_numtostr)(char *buf, const bn_int4096_t n);
extern void FSYMBOL(bn_int4096_numtostr_hex)(char *buf, const bn_int4096_t n);

/* bn_u4096.c */
extern void FSYMBOL(bn_uint4096_zero)(bn_uint4096_t t);
extern void FSYMBOL(bn_uint4096_move)(bn_uint4096_t t, const bn_uint4096_t s);
extern int32 FSYMBOL(bn_uint4096_cmp)(const bn_uint4096_t a, const bn_uint4096_t b);
extern int32 FSYMBOL(bn_uint4097_cmp)(const bn_uint4096_t a, const bn_uint4096_t b);
extern int32 FSYMBOL(bn_uint4096_cmp_1)(const bn_uint4096_t a, uint32 b);
extern void FSYMBOL(bn_uint4096_lsh)(bn_uint4096_t n);
extern void FSYMBOL(bn_uint4097_lsh)(bn_uint4096_t n);
extern void FSYMBOL(bn_uint4096_rsh)(bn_uint4096_t n);
extern void FSYMBOL(bn_uint4097_rsh)(bn_uint4096_t n);
extern void FSYMBOL(bn_uint4096_and)(bn_uint4096_t r, bn_uint4096_t a, bn_uint4096_t b);
extern void FSYMBOL(bn_uint4096_or)(bn_uint4096_t r, bn_uint4096_t a, bn_uint4096_t b);
extern void FSYMBOL(bn_uint4096_xor)(bn_uint4096_t r, bn_uint4096_t a, bn_uint4096_t b);
extern void FSYMBOL(bn_uint4096_not)(bn_uint4096_t r, bn_uint4096_t n);
extern uint32 FSYMBOL(bn_uint4096_add)(bn_uint4096_t r, const bn_uint4096_t a,
		const bn_uint4096_t b);
extern uint32 FSYMBOL(bn_uint4096_add_1)(bn_uint4096_t r, const bn_uint4096_t a, uint32 b);
extern void FSYMBOL(bn_uint4096_sub)(bn_uint4096_t r, const bn_uint4096_t a,
		const bn_uint4096_t b);
extern void FSYMBOL(bn_uint4096_sub_1)(bn_uint4096_t r, const bn_uint4096_t a, uint32 b);
extern uint32 FSYMBOL(bn_uint4096_mul)(bn_uint4096_t r, const bn_uint4096_t a,
		const bn_uint4096_t b);
extern uint32 FSYMBOL(bn_uint4096_mul_1)(bn_uint4096_t r, const bn_uint4096_t a, uint32 b);
extern void FSYMBOL(bn_uint4096_div)(bn_uint4096_t quo, bn_uint4096_t rem,
		const bn_uint4096_t a, const bn_uint4096_t b);
extern void FSYMBOL(bn_uint4096_strtonum)(bn_uint4096_t t, const char *s, char **e,
		int32 b);
extern void FSYMBOL(bn_uint4096_numtostr)(char *buf, const bn_uint4096_t n);
extern void FSYMBOL(bn_uint4096_numtostr_hex)(char *buf, const bn_uint4096_t n);

#ifdef __cplusplus
}
#endif


#endif
