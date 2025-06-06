/* xstdio_snprintf.c - format value to string conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdarg.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdio.h>
#include <libf/sl/internal.h>


/* @def: snprintf */
struct _snout_ctx {
	char *pbuf;
	char *ebuf;
};
/* end */

/* @func: _snout_pad (static) - snprintf output padding callback
* @param1: char   # character
* @param2: int64  # padding size
* @param3: void * # callback arg
* @return: int32  # 0: no error, -1: error
*/
static int32 _snout_pad(char c, int64 n, void *arg) {
	struct _snout_ctx *p = arg;
	while (n-- > 0) {
		*(p->pbuf) = c;
		p->pbuf++;
		if ((p->pbuf + 1) >= p->ebuf)
			return -1;
	}

	return 0;
} /* end */

/* @func: _snout (static) - snprintf output callback
* @param1: char * # input string
* @param2: uint64 # string length
* @param3: void * # callback arg
* @return: int32  # 0: no error, -1: error
*/
static int32 _snout(const char *s, uint64 n, void *arg) {
	struct _snout_ctx *p = arg;
	while (n--) {
		*(p->pbuf) = *s++;
		p->pbuf++;
		if ((p->pbuf + 1) >= p->ebuf)
			return -1;
	}

	return 0;
} /* end */

/* @func: vsnprintf - print formatted output (va_list)
* @param1: char *       # output string buffer
* @param2: uint64       # buffer length
* @param3: const char * # formatted string
* @param4: va_list      # variable argument list
* @return: int32        # 0: no error, -1: format error
*/
int32 XSYMBOL(vsnprintf)(char *buf, uint64 len, const char *fmt,
		va_list ap) {
	struct _snout_ctx p = { buf, buf + len };
	FMT_NEW(ctx, _snout_pad, _snout, &p);

	int32 r = XSYMBOL(internal_fmt)(&ctx, fmt, ap);
	*((char *)(p.pbuf)) = '\0';

	return r;
} /* end */

/* @func: vsnprintf - print formatted output
* @param1: char *       # output string buffer
* @param2: uint64       # buffer length
* @param3: const char * # formatted string
* @paramN: ...          # variable argument
* @return: int32        # 0: no error, -1: format error
*/
int32 XSYMBOL(snprintf)(char *buf, uint64 len, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	struct _snout_ctx p = { buf, buf + len };
	FMT_NEW(ctx, _snout_pad, _snout, &p);

	int32 r = XSYMBOL(internal_fmt)(&ctx, fmt, ap);
	*((char *)(p.pbuf)) = '\0';

	return r;
} /* end */
