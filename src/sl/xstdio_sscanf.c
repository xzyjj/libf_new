/* xstdio_sscanf.c - standard input/output implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/internal.h>


/* @func: vsscanf - string convert to formatted value
* @param1: const char * # string
* @param2: const char * # formatted string
* @param3: va_list      # variable argument list
* @return: int32        # 0: no error, <0: error
*/
int32 XSYMBOL(vsscanf)(const char *s, const char *fmt, va_list ap) {
	FMT_SCANF_NEW(ctx);

	return XSYMBOL(internal_fmt_scanf)(&ctx, s, fmt, ap);
} /* end */

/* @func: sscanf - string convert to formatted value
* @param1: const char * # string
* @param2: const char * # formatted string
* @paramN: ...          # variable argument
* @return: int32        # 0: no error, <0: error
*/
int32 XSYMBOL(sscanf)(const char *s, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	return XSYMBOL(vsscanf)(s, fmt, ap);
} /* end */
