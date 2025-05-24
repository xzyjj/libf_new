/* xabs.h - absolute value integer calculation definitions */

#ifndef LIBF_SL_XABS_H
#define LIBF_SL_XABS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xabs.c */
extern int32 XSYMBOL(abs)(int32 n);
extern int64 XSYMBOL(labs)(int64 n);
extern int64L XSYMBOL(llabs)(int64L n);

#ifdef __cplusplus
}
#endif


#endif
