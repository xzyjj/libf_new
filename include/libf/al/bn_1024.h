/* bn_1024.h - big number calculation (1024bit) definitions */

#ifndef LIBF_AL_BN_1024_H
#define LIBF_AL_BN_1024_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: bn_1024 */
#undef bn_uint1024_t
typedef uint32 bn_uint1024_t[34];
#undef bn_int1024_t
typedef uint32 bn_int1024_t[34];

#undef BN_1024_SIZE
#define BN_1024_SIZE 32
#undef BN_1024_SIG
#define BN_1024_SIG 33
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_i1024.c */
void FSYMBOL(bn_int1024_zero)(bn_int1024_t t);
void FSYMBOL(bn_int1024_move)(bn_int1024_t t, const bn_int1024_t s);
void FSYMBOL(bn_int1024_moveu)(bn_uint1024_t t, const bn_int1024_t s);
void FSYMBOL(bn_int1024_movei)(bn_int1024_t t, const bn_uint1024_t s);
int32 FSYMBOL(bn_int1024_cmp)(const bn_int1024_t a, const bn_int1024_t b);
int32 FSYMBOL(bn_int4097_cmp)(const bn_int1024_t a, const bn_int1024_t b);
int32 FSYMBOL(bn_int1024_cmp_1)(const bn_int1024_t a, uint32 b);
void FSYMBOL(bn_int1024_lsh)(bn_int1024_t n);
void FSYMBOL(bn_int4097_lsh)(bn_int1024_t n);
void FSYMBOL(bn_int1024_rsh)(bn_int1024_t n);
void FSYMBOL(bn_int4097_rsh)(bn_int1024_t n);
void FSYMBOL(bn_int1024_and)(bn_int1024_t r, bn_int1024_t a, bn_int1024_t b);
void FSYMBOL(bn_int1024_or)(bn_int1024_t r, bn_int1024_t a, bn_int1024_t b);
void FSYMBOL(bn_int1024_xor)(bn_int1024_t r, bn_int1024_t a, bn_int1024_t b);
void FSYMBOL(bn_int1024_not)(bn_int1024_t r, bn_int1024_t n);
void FSYMBOL(bn_int1024_add)(bn_int1024_t r, const bn_int1024_t a,
		const bn_int1024_t b);
void FSYMBOL(bn_int1024_sub)(bn_int1024_t r, const bn_int1024_t a,
		const bn_int1024_t b);
void FSYMBOL(bn_int1024_mul)(bn_int1024_t r, const bn_int1024_t a,
		const bn_int1024_t b);
void FSYMBOL(bn_int1024_div)(bn_int1024_t quo, bn_int1024_t rem,
		const bn_int1024_t a, const bn_int1024_t b);
void FSYMBOL(bn_int1024_divmod)(bn_int1024_t quo, bn_int1024_t rem,
		const bn_int1024_t a, const bn_int1024_t b);
void FSYMBOL(bn_int1024_strtonum)(bn_int1024_t t, const char *s, char **e,
		int32 b);
void FSYMBOL(bn_int1024_numtostr)(char *buf, const bn_int1024_t n);

/* bn_u1024.c */
void FSYMBOL(bn_uint1024_zero)(bn_uint1024_t t);
void FSYMBOL(bn_uint1024_move)(bn_uint1024_t t, const bn_uint1024_t s);
int32 FSYMBOL(bn_uint1024_cmp)(const bn_uint1024_t a, const bn_uint1024_t b);
int32 FSYMBOL(bn_uint4097_cmp)(const bn_uint1024_t a, const bn_uint1024_t b);
int32 FSYMBOL(bn_uint1024_cmp_1)(const bn_uint1024_t a, uint32 b);
void FSYMBOL(bn_uint1024_lsh)(bn_uint1024_t n);
void FSYMBOL(bn_uint4097_lsh)(bn_uint1024_t n);
void FSYMBOL(bn_uint1024_rsh)(bn_uint1024_t n);
void FSYMBOL(bn_uint4097_rsh)(bn_uint1024_t n);
void FSYMBOL(bn_uint1024_and)(bn_uint1024_t r, bn_uint1024_t a, bn_uint1024_t b);
void FSYMBOL(bn_uint1024_or)(bn_uint1024_t r, bn_uint1024_t a, bn_uint1024_t b);
void FSYMBOL(bn_uint1024_xor)(bn_uint1024_t r, bn_uint1024_t a, bn_uint1024_t b);
void FSYMBOL(bn_uint1024_not)(bn_uint1024_t r, bn_uint1024_t n);
uint32 FSYMBOL(bn_uint1024_add)(bn_uint1024_t r, const bn_uint1024_t a,
		const bn_uint1024_t b);
uint32 FSYMBOL(bn_uint1024_add_1)(bn_uint1024_t r, const bn_uint1024_t a, uint32 b);
void FSYMBOL(bn_uint1024_sub)(bn_uint1024_t r, const bn_uint1024_t a,
		const bn_uint1024_t b);
void FSYMBOL(bn_uint1024_sub_1)(bn_uint1024_t r, const bn_uint1024_t a, uint32 b);
uint32 FSYMBOL(bn_uint1024_mul)(bn_uint1024_t r, const bn_uint1024_t a,
		const bn_uint1024_t b);
uint32 FSYMBOL(bn_uint1024_mul_1)(bn_uint1024_t r, const bn_uint1024_t a, uint32 b);
void FSYMBOL(bn_uint1024_div)(bn_uint1024_t quo, bn_uint1024_t rem,
		const bn_uint1024_t a, const bn_uint1024_t b);
void FSYMBOL(bn_uint1024_strtonum)(bn_uint1024_t t, const char *s, char **e,
		int32 b);
void FSYMBOL(bn_uint1024_numtostr)(char *buf, const bn_uint1024_t n);

#ifdef __cplusplus
}
#endif


#endif
