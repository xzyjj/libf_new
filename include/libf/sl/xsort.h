/* xsort.h - data table sorting definitions */

#ifndef LIBF_SL_XSORT_H
#define LIBF_SL_XSORT_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xsort.c */
extern void XSYMBOL(qsort)(void *b, uint64 n, uint64 w,
		int32 (*cmp)(const void *, const void *));
extern void *XSYMBOL(bsearch)(const void *k, const void *b, uint64 n, uint64 w,
		int32 (*cmp)(const void *, const void *));

#ifdef __cplusplus
}
#endif


#endif
