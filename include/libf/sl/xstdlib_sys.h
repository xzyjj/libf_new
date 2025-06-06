/* xstdlib_sys.h - standard library definitions */

#ifndef LIBF_SL_XSTDLIB_SYS_H
#define LIBF_SL_XSTDLIB_SYS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xstdlib_malloc_sys.c */
extern void *XSYMBOL(malloc)(uint64 size);
extern void XSYMBOL(free)(void *p);
extern void XSYMBOL(free_all)(void);
extern uint64 XSYMBOL(malloc_idle)(int32 type);

#ifdef __cplusplus
}
#endif


#endif
