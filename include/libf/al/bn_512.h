/* bn_512.h - big number calculation (512bit) definitions */

#ifndef LIBF_AL_BN_512_H
#define LIBF_AL_BN_512_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: bn_512 */
#undef bn_uint512_t
typedef uint32 bn_uint512_t[18];
#undef bn_int512_t
typedef uint32 bn_int512_t[18];

#undef BN_512_SIZE
#define BN_512_SIZE 16
#undef BN_512_SIG
#define BN_512_SIG 17
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_i512.c */
void FSYMBOL(bn_int512_zero)(bn_int512_t t);
void FSYMBOL(bn_int512_move)(bn_int512_t t, const bn_int512_t s);
void FSYMBOL(bn_int512_moveu)(bn_uint512_t t, const bn_int512_t s);
void FSYMBOL(bn_int512_movei)(bn_int512_t t, const bn_uint512_t s);
int32 FSYMBOL(bn_int512_cmp)(const bn_int512_t a, const bn_int512_t b);
int32 FSYMBOL(bn_int513_cmp)(const bn_int512_t a, const bn_int512_t b);
int32 FSYMBOL(bn_int512_cmp_1)(const bn_int512_t a, uint32 b);
void FSYMBOL(bn_int512_lsh)(bn_int512_t n);
void FSYMBOL(bn_int513_lsh)(bn_int512_t n);
void FSYMBOL(bn_int512_rsh)(bn_int512_t n);
void FSYMBOL(bn_int513_rsh)(bn_int512_t n);
void FSYMBOL(bn_int512_and)(bn_int512_t r, bn_int512_t a, bn_int512_t b);
void FSYMBOL(bn_int512_or)(bn_int512_t r, bn_int512_t a, bn_int512_t b);
void FSYMBOL(bn_int512_xor)(bn_int512_t r, bn_int512_t a, bn_int512_t b);
void FSYMBOL(bn_int512_not)(bn_int512_t r, bn_int512_t n);
void FSYMBOL(bn_int512_add)(bn_int512_t r, const bn_int512_t a,
		const bn_int512_t b);
void FSYMBOL(bn_int512_sub)(bn_int512_t r, const bn_int512_t a,
		const bn_int512_t b);
void FSYMBOL(bn_int512_mul)(bn_int512_t r, const bn_int512_t a,
		const bn_int512_t b);
void FSYMBOL(bn_int512_div)(bn_int512_t quo, bn_int512_t rem,
		const bn_int512_t a, const bn_int512_t b);
void FSYMBOL(bn_int512_divmod)(bn_int512_t quo, bn_int512_t rem,
		const bn_int512_t a, const bn_int512_t b);
void FSYMBOL(bn_int512_strtonum)(bn_int512_t t, const char *s, char **e,
		int32 b);
void FSYMBOL(bn_int512_numtostr)(char *buf, const bn_int512_t n);

/* bn_u512.c */
void FSYMBOL(bn_uint512_zero)(bn_uint512_t t);
void FSYMBOL(bn_uint512_move)(bn_uint512_t t, const bn_uint512_t s);
int32 FSYMBOL(bn_uint512_cmp)(const bn_uint512_t a, const bn_uint512_t b);
int32 FSYMBOL(bn_uint513_cmp)(const bn_uint512_t a, const bn_uint512_t b);
int32 FSYMBOL(bn_uint512_cmp_1)(const bn_uint512_t a, uint32 b);
void FSYMBOL(bn_uint512_lsh)(bn_uint512_t n);
void FSYMBOL(bn_uint513_lsh)(bn_uint512_t n);
void FSYMBOL(bn_uint512_rsh)(bn_uint512_t n);
void FSYMBOL(bn_uint513_rsh)(bn_uint512_t n);
void FSYMBOL(bn_uint512_and)(bn_uint512_t r, bn_uint512_t a, bn_uint512_t b);
void FSYMBOL(bn_uint512_or)(bn_uint512_t r, bn_uint512_t a, bn_uint512_t b);
void FSYMBOL(bn_uint512_xor)(bn_uint512_t r, bn_uint512_t a, bn_uint512_t b);
void FSYMBOL(bn_uint512_not)(bn_uint512_t r, bn_uint512_t n);
uint32 FSYMBOL(bn_uint512_add)(bn_uint512_t r, const bn_uint512_t a,
		const bn_uint512_t b);
uint32 FSYMBOL(bn_uint512_add_1)(bn_uint512_t r, const bn_uint512_t a, uint32 b);
void FSYMBOL(bn_uint512_sub)(bn_uint512_t r, const bn_uint512_t a,
		const bn_uint512_t b);
void FSYMBOL(bn_uint512_sub_1)(bn_uint512_t r, const bn_uint512_t a, uint32 b);
uint32 FSYMBOL(bn_uint512_mul)(bn_uint512_t r, const bn_uint512_t a,
		const bn_uint512_t b);
uint32 FSYMBOL(bn_uint512_mul_1)(bn_uint512_t r, const bn_uint512_t a, uint32 b);
void FSYMBOL(bn_uint512_div)(bn_uint512_t quo, bn_uint512_t rem,
		const bn_uint512_t a, const bn_uint512_t b);
void FSYMBOL(bn_uint512_strtonum)(bn_uint512_t t, const char *s, char **e,
		int32 b);
void FSYMBOL(bn_uint512_numtostr)(char *buf, const bn_uint512_t n);

#ifdef __cplusplus
}
#endif


#endif
