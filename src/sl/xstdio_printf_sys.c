/* xstdio_printf_sys.c - standard input/output implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdarg.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdio_sys.h>
#include <libf/sl/internal.h>

#include <stdio.h>


/* @func: _out_pad (static) - printf output padding callback
* @param1: char   # character
* @param2: int64  # padding size
* @param3: void * # callback arg
* @return: int32  # 0: no error, -1: error
*/
static int32 _out_pad(char c, int64 n, void *arg) {
	char buf[32];
	for (; n > 32; n -= 32) {
		XSYMBOL(memset)(buf, c, 32);
		fwrite(buf, 1, 32, arg);
	}
	if (n > 0) {
		XSYMBOL(memset)(buf, c, n);
		fwrite(buf, 1, n, arg);
	}

	return 0;
} /* end */

/* @func: _out (static) - printf output callback
* @param1: char * # input string
* @param2: uint64 # string length
* @param3: void * # callback arg
* @return: int32  # 0: no error, -1: error
*/
static int32 _out(const char *s, uint64 n, void *arg) {
	fwrite(s, 1, n, arg);

	return 0;
} /* end */

/* @func: vfprintf - print formatted output (va_list and FILE)
* @param1: const char * # formatted string
* @param2: va_list      # variable argument list
* @return: int32        # 0: no error, -1: format error
*/
int32 XSYMBOL(vfprintf)(FILE *fp, const char *fmt, va_list ap) {
	FMT_PRINTF_NEW(ctx, _out_pad, _out, fp);

	return XSYMBOL(internal_fmt_printf)(&ctx, fmt, ap);
} /* end */

/* @func: fprintf - print formatted output (FILE)
* @param1: const char * # formatted string
* @paramN: ...          # variable argument
* @return: int32        # 0: no error, -1: format error
*/
int32 XSYMBOL(fprintf)(FILE *fp, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	return XSYMBOL(vfprintf)(fp, fmt, ap);
} /* end */

/* @func: vprintf - print formatted output (va_list)
* @param1: const char * # formatted string
* @param2: va_list      # variable argument list
* @return: int32        # 0: no error, -1: format error
*/
int32 XSYMBOL(vprintf)(const char *fmt, va_list ap) {
	return XSYMBOL(vfprintf)(stdout, fmt, ap);
} /* end */

/* @func: printf - print formatted output
* @param1: const char * # formatted string
* @paramN: ...          # variable argument
* @return: int32        # 0: no error, -1: format error
*/
int32 XSYMBOL(printf)(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	return XSYMBOL(vfprintf)(stdout, fmt, ap);
} /* end */
