/* xatomic.h - atomic operation definitions */

#ifndef LIBF_SL_XATOMIC_H
#define LIBF_SL_XATOMIC_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xatomic.S */
int32 XSYMBOL(atomic_cas)(volatile int32 *p, int32 old, int32 _new);

#ifdef __cplusplus
}
#endif


#endif
