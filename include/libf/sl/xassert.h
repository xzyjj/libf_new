/* xassert.h - program assertion definitions */

#ifndef LIBF_SL_XASSERT_H
#define LIBF_SL_XASSERT_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: */
#undef assert
#ifdef NDEBUG
#	define xassert() ((void)0)
#else
/*
#	define xassert(x) ((void)((x) \
		|| (xassert_exit(#x, __FILE__, __LINE__, __func__), 0)))
*/
#	define xassert(x) ((void)((x) || (XSYMBOL(crash)(), 0)))
#endif
/* end */

/* @func: crash (static) - core dump (segmentation fault) */
inline static void XSYMBOL(crash)(void) {
	*((volatile char *)0) = 0;
} /* end */


#endif
