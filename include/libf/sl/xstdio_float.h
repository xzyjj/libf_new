/* xstdio_float.h - standard input/output definitions */

#ifndef LIBF_SL_XSTDIO_FLOAT_H
#define LIBF_SL_XSTDIO_FLOAT_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstdarg.h>


/* @def: fmt_vprintf */
#undef fmt_vprintf_ctx
struct fmt_vprintf_ctx {
	int64 align;
	int64 precise;
	uint32 flags;
	char specifiers;
	union {
		int64L _i64L;
		uint64L _u64L;
		float64 _f64;
		char _char;
		const char *_str;
	} va;
	void *arg;
	/* character, size, arg */
	int32 (*call_pad)(char, int64, void *);
	/* string, length, arg */
	int32 (*call_out)(const char *, uint64, void *);
};

#undef FMT_VPRINTF_NEW
#define FMT_VPRINTF_NEW(name, _call_pad, _call_out, _arg) \
	struct fmt_vprintf_ctx name = { \
		.call_pad = _call_pad, \
		.call_out = _call_out, \
		.arg = _arg \
	}

#undef FMT_VPRINTF_INIT
#define FMT_VPRINTF_INIT(name, _call_pad, _call_out, _arg) \
	(name)->call_pad = _call_pad; \
	(name)->call_out = _call_out; \
	(name)->arg = _arg
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xstdio_fmt_float.c */
extern int32 XSYMBOL(fmt_vprintf)(struct fmt_vprintf_ctx *ctx,
		const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif


#endif
