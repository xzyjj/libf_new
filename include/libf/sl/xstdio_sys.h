/* xstdio_sys.h - standard input/output definitions */

#ifndef LIBF_SL_XSTDIO_SYS_H
#define LIBF_SL_XSTDIO_SYS_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdarg.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* xstdio_printf_sys.c */
extern int32 XSYMBOL(vfprintf)(FILE *fp, const char *fmt, va_list ap);
extern int32 XSYMBOL(fprintf)(FILE *fp, const char *fmt, ...);
extern int32 XSYMBOL(vprintf)(const char *fmt, va_list ap);
extern int32 XSYMBOL(printf)(const char *fmt, ...);

#ifdef __cplusplus
}
#endif


#endif
