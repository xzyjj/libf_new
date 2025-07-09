/* xstdio.h - standard input/output definitions */

#ifndef LIBF_SL_XSTDIO_H
#define LIBF_SL_XSTDIO_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdarg.h>


#ifdef __cplusplus
extern "C" {
#endif

/* xstdio_snprintf.c */
extern int32 XSYMBOL(vsnprintf)(char *buf, uint64 len, const char *fmt,
		va_list ap);
extern int32 XSYMBOL(snprintf)(char *buf, uint64 len, const char *fmt, ...);

/* xstdio_sscanf.c */
extern int32 XSYMBOL(vsscanf)(const char *s, const char *fmt, va_list ap);
extern int32 XSYMBOL(sscanf)(const char *s, const char *fmt, ...);

#ifdef __cplusplus
}
#endif


#endif
