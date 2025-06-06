/* xstdlib.h - standard library definitions */

#ifndef LIBF_SL_XSTDLIB_H
#define LIBF_SL_XSTDLIB_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>


/* @def: random */
#undef random_ctx
struct random_ctx {
	int32 type;
	int32 *state;
	int32 *fptr;
	int32 *bptr;
	int32 *eptr;
};

/* LCG (Linear Congruential Generator) */
#undef RANDOM_TYPE0_NEW
#define RANDOM_TYPE0_NEW(name, seed) \
	int32 name##_random_tab0[1] = { \
		seed \
		}; \
	struct random_ctx name = { \
		.type = 0, \
		.state = name##_random_tab0 \
		}

/* LFSR (Linear Feedback Shift Register)
* x^31 + x^3 + 1
*/
#undef RANDOM_TYPE1_NEW
#define RANDOM_TYPE1_NEW(name) \
	int32 name##_random_tab1[32] = { \
		-1726662223, 379960547, 1735697613, 1040273694, \
		1313901226, 1627687941, -179304937, -2073333483, \
		1780058412, -1989503057, -615974602, 344556628, \
		939512070, -1249116260, 1507946756, -812545463, \
		154635395, 1388815473, -1926676823, 525320961, \
		-1009028674, 968117788, -123449607, 1284210865, \
		435012392, -2017506339, -911064859, -370259173, \
		1132637927, 1398500161, -205601318, 0 \
		}; \
	struct random_ctx name = { \
		.type = 1, \
		.state = name##_random_tab1, \
		.fptr = &name##_random_tab1[3], \
		.bptr = &name##_random_tab1[0], \
		.eptr = &name##_random_tab1[31] \
		}
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xstdlib_a64l.c */
extern uint64 XSYMBOL(a64l)(const char *s);
extern char *XSYMBOL(l64a_r)(char *b, uint64 n);

/* xstdlib_abs.c */
extern int32 XSYMBOL(abs)(int32 n);
extern int64 XSYMBOL(labs)(int64 n);
extern int64L XSYMBOL(llabs)(int64L n);

/* xstdlib_atof.c */
extern float64 XSYMBOL(atof)(const char *s);
extern float32 XSYMBOL(strtof)(const char *s, char **e);
extern float64 XSYMBOL(strtod)(const char *s, char **e);

/* xstdlib_atoi.c */
extern int32 XSYMBOL(atoi)(const char *s);
extern int64 XSYMBOL(atol)(const char *s);
extern int64L XSYMBOL(atoll)(const char *s);
extern int64 XSYMBOL(strtol)(const char *s, char **e, int32 b);
extern uint64 XSYMBOL(strtoul)(const char *s, char **e, int32 b);
extern int64L XSYMBOL(strtoll)(const char *s, char **e, int32 b);
extern uint64L XSYMBOL(strtoull)(const char *s, char **e, int32 b);

/* xstdlib_getsubopt.c */
extern int32 XSYMBOL(getsubopt)(char **optp, char *const *toks, char **valp);

/* xstdlib_qsort.c */
extern void XSYMBOL(qsort)(void *b, uint64 n, uint64 w,
		int32 (*cmp)(const void *, const void *));
extern void *XSYMBOL(bsearch)(const void *k, const void *b, uint64 n, uint64 w,
		int32 (*cmp)(const void *, const void *));

/* xstdlib_random.c */
extern int32 XSYMBOL(random_r)(struct random_ctx *p, int32 *v);

#ifdef __cplusplus
}
#endif


#endif
