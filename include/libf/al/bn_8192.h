/* bn_8192.h - big number calculation (8192bit) definitions */

#ifndef LIBF_AL_BN_8192_H
#define LIBF_AL_BN_8192_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: bn_8192 */
#undef bn_uint8192_t
typedef uint32 bn_uint8192_t[258];
#undef bn_int8192_t
typedef uint32 bn_int8192_t[258];

#undef BN_8192_SIZE
#define BN_8192_SIZE 256
#undef BN_8192_SIG
#define BN_8192_SIG 257
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bn_i8192.c */
extern void FSYMBOL(bn_int8192_zero)(bn_int8192_t t);
extern uint32 FSYMBOL(bn_int8192_bits)(const bn_int8192_t n);
extern void FSYMBOL(bn_int8192_move)(bn_int8192_t t, const bn_int8192_t s);
extern void FSYMBOL(bn_int8192_moveu)(bn_uint8192_t t, const bn_int8192_t s);
extern void FSYMBOL(bn_int8192_movei)(bn_int8192_t t, const bn_uint8192_t s);
extern int32 FSYMBOL(bn_int8192_cmp)(const bn_int8192_t a, const bn_int8192_t b);
extern int32 FSYMBOL(bn_int8193_cmp)(const bn_int8192_t a, const bn_int8192_t b);
extern int32 FSYMBOL(bn_int8192_cmp_1)(const bn_int8192_t a, uint32 b);
extern void FSYMBOL(bn_int8192_lsh)(bn_int8192_t n);
extern void FSYMBOL(bn_int8193_lsh)(bn_int8192_t n);
extern void FSYMBOL(bn_int8192_rsh)(bn_int8192_t n);
extern void FSYMBOL(bn_int8193_rsh)(bn_int8192_t n);
extern void FSYMBOL(bn_int8192_and)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_or)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_xor)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_not)(bn_int8192_t r,
		const bn_int8192_t n);
extern void FSYMBOL(bn_int8192_add)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_sub)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_mul)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_div)(bn_int8192_t quo, bn_int8192_t rem,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_divmod)(bn_int8192_t quo, bn_int8192_t rem,
		const bn_int8192_t a, const bn_int8192_t b);
extern void FSYMBOL(bn_int8192_strtonum)(bn_int8192_t t, const char *s, char **e,
		int32 b);
extern void FSYMBOL(bn_int8192_numtostr)(char *buf, const bn_int8192_t n);
extern void FSYMBOL(bn_int8192_numtostr_hex)(char *buf, const bn_int8192_t n);

/* bn_u8192.c */
extern void FSYMBOL(bn_uint8192_zero)(bn_uint8192_t t);
extern uint32 FSYMBOL(bn_uint8192_bits)(const bn_uint8192_t n);
extern void FSYMBOL(bn_uint8192_move)(bn_uint8192_t t, const bn_uint8192_t s);
extern int32 FSYMBOL(bn_uint8192_cmp)(const bn_uint8192_t a, const bn_uint8192_t b);
extern int32 FSYMBOL(bn_uint8193_cmp)(const bn_uint8192_t a, const bn_uint8192_t b);
extern int32 FSYMBOL(bn_uint8192_cmp_1)(const bn_uint8192_t a, uint32 b);
extern void FSYMBOL(bn_uint8192_lsh)(bn_uint8192_t n);
extern void FSYMBOL(bn_uint8193_lsh)(bn_uint8192_t n);
extern void FSYMBOL(bn_uint8192_rsh)(bn_uint8192_t n);
extern void FSYMBOL(bn_uint8193_rsh)(bn_uint8192_t n);
extern void FSYMBOL(bn_uint8192_and)(bn_uint8192_t r,
		const bn_uint8192_t a, const bn_uint8192_t b);
extern void FSYMBOL(bn_uint8192_or)(bn_uint8192_t r,
		const bn_uint8192_t a, const bn_uint8192_t b);
extern void FSYMBOL(bn_uint8192_xor)(bn_uint8192_t r,
		const bn_uint8192_t a, const bn_uint8192_t b);
extern void FSYMBOL(bn_uint8192_not)(bn_uint8192_t r,
		const bn_uint8192_t n);
extern uint32 FSYMBOL(bn_uint8192_add)(bn_uint8192_t r,
		const bn_uint8192_t a, const bn_uint8192_t b);
extern uint32 FSYMBOL(bn_uint8192_add_1)(bn_uint8192_t r,
		const bn_uint8192_t a, uint32 b);
extern void FSYMBOL(bn_uint8192_sub)(bn_uint8192_t r,
		const bn_uint8192_t a, const bn_uint8192_t b);
extern void FSYMBOL(bn_uint8192_sub_1)(bn_uint8192_t r,
		const bn_uint8192_t a, uint32 b);
extern uint32 FSYMBOL(bn_uint8192_mul)(bn_uint8192_t r,
		const bn_uint8192_t a, const bn_uint8192_t b);
extern uint32 FSYMBOL(bn_uint8192_mul_1)(bn_uint8192_t r,
		const bn_uint8192_t a, uint32 b);
extern void FSYMBOL(bn_uint8192_div)(bn_uint8192_t quo, bn_uint8192_t rem,
		const bn_uint8192_t a, const bn_uint8192_t b);
extern void FSYMBOL(bn_uint8192_strtonum)(bn_uint8192_t t, const char *s, char **e,
		int32 b);
extern void FSYMBOL(bn_uint8192_numtostr)(char *buf, const bn_uint8192_t n);
extern void FSYMBOL(bn_uint8192_numtostr_hex)(char *buf, const bn_uint8192_t n);

#ifdef __cplusplus
}
#endif


#endif
