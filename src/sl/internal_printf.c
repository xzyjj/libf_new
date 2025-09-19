/* internal_printf.c - format value to string conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdarg.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdlib.h>
#include <libf/sl/internal.h>


/* @def: fmt_printf */
#undef FG_LONG
#define FG_LONG 0x01
#undef FG_LONG_LONG
#define FG_LONG_LONG 0x02
#undef FG_SHORT
#define FG_SHORT 0x04
#undef FG_CHAR
#define FG_CHAR 0x08
#undef FG_LONG_DOUBLE
#define FG_LONG_DOUBLE 0x10
#undef FG_ALIGN_LEFT
#define FG_ALIGN_LEFT 0x20
#undef FG_ALIGN_RIGHT
#define FG_ALIGN_RIGHT 0x40
#undef FG_ALIGN_RIGHT_ZERO
#define FG_ALIGN_RIGHT_ZERO 0x80
#undef FG_BASE_PREFIX
#define FG_BASE_PREFIX 0x100
#undef FG_POSITIVE_NEGATIVE
#define FG_POSITIVE_NEGATIVE 0x200
/* end */

/* @func: _fmt_printf_di (static) - printf output number
* @param1: struct fmt_printf_ctx * # fmt-printf struct context
* @return: int32                   # 0: no error, <0: error
*/
static int32 _fmt_printf_di(struct fmt_printf_ctx *ctx) {
	char buf[32];
	int32 len, len_s, neg,
		pre_len = 0, ar_len = 0,
		arz_len = 0, al_len = 0;
	int64L v = ctx->va._i64L;

	neg = (v < 0) ? 1 : 0;
	v = XSYMBOL(llabs)(v);
	len = len_s = XSYMBOL(internal_ulltostr_d)(0, buf, v);

	if (neg)
		len++;
	if (ctx->precise > 0)
		pre_len = ctx->precise - len;
	if (ctx->flags & FG_ALIGN_RIGHT)
		ar_len = ctx->align - len - pre_len;
	if (ctx->flags & FG_ALIGN_RIGHT_ZERO) {
		arz_len = ctx->align - len;
		if (ctx->precise > 0)
			arz_len -= pre_len;
	}
	if (ctx->flags & FG_ALIGN_LEFT)
		al_len = ctx->align - len - pre_len;

	/* align right */
	if (ctx->call_pad(' ', ar_len, ctx->arg))
		return -2;
	/* negative and positive */
	if (neg) {
		if (ctx->call_pad('-', 1, ctx->arg))
			return -2;
	} else if (ctx->flags & FG_POSITIVE_NEGATIVE) {
		if (ctx->call_pad('+', 1, ctx->arg))
			return -2;
	}
	/* align right zero */
	if (ctx->call_pad('0', arz_len, ctx->arg))
		return -2;
	/* precise */
	if (ctx->call_pad('0', pre_len, ctx->arg))
		return -2;
	/* output */
	if (ctx->call_out(buf, len_s, ctx->arg))
		return -2;
	/* align left */
	if (ctx->call_pad(' ', al_len, ctx->arg))
		return -2;

	return 0;
} /* end */

/* @func: _fmt_printf_ouxXp (static) - printf output number
* @param1: struct fmt_printf_ctx * # fmt-printf struct context
* @return: int32                   # 0: no error, <0: error
*/
static int32 _fmt_printf_ouxXp(struct fmt_printf_ctx *ctx) {
	const char *bp_str = NULL;
	char buf[32];
	int32 len, len_s,
		pre_len = 0, ar_len = 0,
		arz_len = 0, al_len = 0;
	uint64L v = ctx->va._u64L;

	switch (ctx->specifiers) {
		case 'x': case 'p':
			len = len_s = XSYMBOL(internal_ulltostr_x)(0, buf, v); 
			break;
		case 'X':
			len = len_s = XSYMBOL(internal_ulltostr_X)(0, buf, v);
			break;
		case 'o':
			len = len_s = XSYMBOL(internal_ulltostr_o)(0, buf, v);
			break;
		case 'u':
			len = len_s = XSYMBOL(internal_ulltostr_d)(0, buf, v);
			break;
		default:
			return -1;
	}

	if (ctx->precise > 0)
		pre_len = ctx->precise - len;
	if (ctx->flags & FG_BASE_PREFIX) {
		switch (ctx->specifiers) {
			case 'x': case 'p':
				bp_str = "0x";
				len += 2;
				break;
			case 'X':
				bp_str = "0X";
				len += 2;
				break;
			case 'o':
				bp_str = "0";
				len += 1;
				break;
			default:
				break;
		}
	}
	if (ctx->flags & FG_ALIGN_RIGHT)
		ar_len = ctx->align - len - pre_len;
	if (ctx->flags & FG_ALIGN_RIGHT_ZERO) {
		arz_len = ctx->align - len;
		if (ctx->precise > 0)
			arz_len -= pre_len;
	}
	if (ctx->flags & FG_ALIGN_LEFT)
		al_len = ctx->align - len - pre_len;

	/* align right */
	if (ctx->call_pad(' ', ar_len, ctx->arg))
		return -2;
	/* base prefix */
	if (bp_str && ctx->call_out(bp_str,
			XSYMBOL(strlen)(bp_str),
			ctx->arg))
		return -2;
	/* align right zero */
	if (ctx->call_pad('0', arz_len, ctx->arg))
		return -2;
	/* precise */
	if (ctx->call_pad('0', pre_len, ctx->arg))
		return -2;
	/* output */
	if (ctx->call_out(buf, len_s, ctx->arg))
		return -2;
	/* align left */
	if (ctx->call_pad(' ', al_len, ctx->arg))
		return -2;

	return 0;
} /* end */

/* @func: _fmt_printf_cs (static) - printf output char/string
* @param1: struct fmt_printf_ctx * # fmt-printf struct context
* @return: int32                   # 0: no error, <0: error
*/
static int32 _fmt_printf_cs(struct fmt_printf_ctx *ctx) {
	const char *bp_str = NULL;
	char buf[12];
	int32 len, len_s, ar_len = 0,
		arz_len = 0, al_len = 0;

	switch (ctx->specifiers) {
		case 'c':
			buf[0] = ctx->va._char;
			buf[1] = '\0';
			bp_str = buf;
			len = len_s = 1;
			break;
		case 's':
			bp_str = ctx->va._str;
			if (!bp_str)
				bp_str = "(null)";
			len = len_s = XSYMBOL(strlen)(bp_str);
			break;
		default:
			return -1;
	}

	if (ctx->flags & FG_ALIGN_RIGHT)
		ar_len = ctx->align - len;
	if (ctx->flags & FG_ALIGN_RIGHT_ZERO)
		arz_len = ctx->align - len;
	if (ctx->flags & FG_ALIGN_LEFT)
		al_len = ctx->align - len;

	/* align right */
	if (ctx->call_pad(' ', ar_len, ctx->arg))
		return -2;
	/* align right zero */
	if (ctx->call_pad('0', arz_len, ctx->arg))
		return -2;
	/* output */
	if (ctx->call_out(bp_str, len_s, ctx->arg))
		return -2;
	/* align left */
	if (ctx->call_pad(' ', al_len, ctx->arg))
		return -2;

	return 0;
} /* end */

/* @func: _fmt_printf_f (static) - printf output floating
* @param1: struct fmt_printf_ctx * # fmt-printf struct context
* @return: int32                   # 0: no error, <0: error
*/
static int32 _fmt_printf_f(struct fmt_printf_ctx *ctx) {
	char buf[FLTO_LENMAX];
	int32 len, len_s, neg,
		pre_len = 0, ar_len = 0,
		arz_len = 0, al_len = 0;
	float64 v = ctx->va._f64;

	neg = (v < 0) ? 1 : 0;
	switch (ctx->specifiers) {
		case 'e': case 'E': case 'g':
		case 'G': case 'a': case 'A':
		case 'f': case 'F':
			len = len_s = XSYMBOL(internal_fltostr_num)(0, buf, v,
				ctx->precise);
			break;
		default:
			return -1;
	}

	if (neg)
		len++;
	if (ctx->precise > FLTO_PREMAX)
		pre_len = ctx->precise - FLTO_PREMAX;
	if (ctx->flags & FG_ALIGN_RIGHT)
		ar_len = ctx->align - len - pre_len;
	if (ctx->flags & FG_ALIGN_RIGHT_ZERO) {
		arz_len = ctx->align - len;
		if (ctx->precise > 0)
			arz_len -= pre_len;
	}
	if (ctx->flags & FG_ALIGN_LEFT)
		al_len = ctx->align - len - pre_len;

	/* align right */
	if (ctx->call_pad(' ', ar_len, ctx->arg))
		return -2;
	/* negative and positive */
	if (neg) {
		if (ctx->call_pad('-', 1, ctx->arg))
			return -2;
	} else if (ctx->flags & FG_POSITIVE_NEGATIVE) {
		if (ctx->call_pad('+', 1, ctx->arg))
			return -2;
	}
	/* align right zero */
	if (ctx->call_pad('0', arz_len, ctx->arg))
		return -2;
	/* output */
	if (ctx->call_out(buf, len_s, ctx->arg))
		return -2;
	/* precise */
	if (ctx->call_pad('0', pre_len, ctx->arg))
		return -2;
	/* align left */
	if (ctx->call_pad(' ', al_len, ctx->arg))
		return -2;

	return 0;
} /* end */

/* @func: internal_fmt_printf - formatted value parse and convert string
* @param1: struct fmt_printf_ctx * # fmt struct context
* @param2: const char *            # formatted string
* @param3: va_list                 # variable argument list
* @return: int32                   # 0: no error, <0: error
*/
int32 XSYMBOL(internal_fmt_printf)(struct fmt_printf_ctx *ctx,
		const char *fmt, va_list ap) {
	for (; *fmt != '\0'; fmt++) {
		if (*fmt == '%') {
			fmt++;
			if (*fmt == '%') {
				if (ctx->call_pad(*fmt, 1, ctx->arg))
					return -2;
				continue;
			}
		} else {
			if (ctx->call_pad(*fmt, 1, ctx->arg))
				return -2;
			continue;
		}
		ctx->align = 0;
		ctx->precise = 0;
		ctx->flags = 0;

		while (1) {
			switch (*fmt) {
				case '#': /* base prefix */
					ctx->flags |= FG_BASE_PREFIX;
					fmt++;
					break;
				case '+': /* show positive and negative */
					ctx->flags |= FG_POSITIVE_NEGATIVE;
					fmt++;
					break;
				case '-': /* align left */
					ctx->flags |= FG_ALIGN_LEFT;
					fmt++;
					break;
				case '0': /* zero padding */
					ctx->flags |= FG_ALIGN_RIGHT_ZERO;
					fmt++;
					break;
				default:
					goto e;
			}
		}
e:

#undef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

		/* align left and right */
		if (*fmt == '*' || (*fmt >= '0' && *fmt <= '9')) {
			if (ctx->flags & FG_ALIGN_LEFT) {
				ctx->flags &= ~FG_ALIGN_RIGHT_ZERO;
			} else { /* align right */
				ctx->flags |= FG_ALIGN_RIGHT;
			}
			if (ctx->flags & FG_ALIGN_RIGHT_ZERO)
				ctx->flags &= ~FG_ALIGN_RIGHT;

			if (*fmt == '*') { /* dynamic */
				ctx->align = va_arg(ap, int32);
				fmt++;
			} else {
				ctx->align = XSYMBOL(strtol)(fmt,
					(char **)&fmt, 10);
			}
			ctx->align = MAX(ctx->align, 0);
		}

		/* precise */
		if (*fmt == '.') {
			fmt++;
			if (*fmt == '*') { /* dynamic */
				ctx->precise = va_arg(ap, int32);
				fmt++;
			} else {
				ctx->precise = XSYMBOL(strtol)(fmt,
					(char **)&fmt, 10);
			}
			ctx->precise = MAX(ctx->precise, 0);
		}

		/* length modifier */
		switch (*fmt) {
			case 'L': /* long double */
				ctx->flags |= FG_LONG_DOUBLE;
				fmt++;
				break;
			case 'l':
				fmt++;
				if (*fmt == 'l') { /* long long */
					ctx->flags |= FG_LONG_LONG;
				} else { /* long */
					ctx->flags |= FG_LONG;
					fmt--;
				}
				fmt++;
				break;
			case 'h':
				fmt++;
				if (*fmt == 'h') { /* char */
					ctx->flags |= FG_CHAR;
				} else { /* short */
					ctx->flags |= FG_SHORT;
					fmt--;
				}
				fmt++;
				break;
			case 'z': /* size_t */
			case 't': /* ptrdiff_t */
				ctx->flags |= FG_LONG;
				fmt++;
				break;
			default:
				break;
		}

		/* conversion specifiers */
		ctx->specifiers = *fmt;
		switch (*fmt) {
			case 'n':
				return -1;
			case 'd': /* decimal */
			case 'i':
				if (ctx->flags & FG_LONG) {
					ctx->va._i64L = va_arg(ap, int64);
				} else if (ctx->flags & FG_LONG_LONG) {
					ctx->va._i64L = va_arg(ap, int64L);
				} else if (ctx->flags & FG_SHORT) {
					ctx->va._i64L = (int16)va_arg(ap, int32);
				} else if (ctx->flags & FG_CHAR) {
					ctx->va._i64L = (int8)va_arg(ap, int32);
				} else {
					ctx->va._i64L = va_arg(ap, int32);
				}
				break;
			case 'o': /* octal */
			case 'u': /* decimal */
			case 'x': /* hexadecimal */
			case 'X':
			case 'p': /* pointer */
				if (ctx->flags & FG_LONG || *fmt == 'p') {
					ctx->va._u64L = va_arg(ap, uint64);
				} else if (ctx->flags & FG_LONG_LONG) {
					ctx->va._u64L = va_arg(ap, uint64L);
				} else if (ctx->flags & FG_SHORT) {
					ctx->va._u64L = (uint16)va_arg(ap, uint32);
				} else if (ctx->flags & FG_CHAR) {
					ctx->va._u64L = (uint8)va_arg(ap, uint32);
				} else {
					ctx->va._u64L = va_arg(ap, uint32);
				}
				break;
			case 'c': /* character */
				ctx->va._char = (char)va_arg(ap, uint32);
				break;
			case 's': /* string */
				ctx->va._str = va_arg(ap, char *);
				break;
			case 'e': /* floating-point */
			case 'E':
			case 'f': case 'F': case 'g':
			case 'G': case 'a': case 'A':
				if (!ctx->precise)
					ctx->precise = 6;
				ctx->va._f64 = va_arg(ap, float64);
				break;
			default:
				return -1;
		}

		switch (ctx->specifiers) {
			case 'n':
				return -1;
			case 'd': /* decimal */
			case 'i':
				if (_fmt_printf_di(ctx))
					return -2;
				break;
			case 'o': /* octal */
			case 'u': /* decimal */
			case 'x': /* hexadecimal */
			case 'X':
			case 'p': /* pointer */
				if (_fmt_printf_ouxXp(ctx))
					return -2;
				break;
			case 'c': /* character */
			case 's': /* string */
				if (_fmt_printf_cs(ctx))
					return -2;
				break;
			case 'e': /* floating-point */
			case 'E':
			case 'f': case 'F': case 'g':
			case 'G': case 'a': case 'A':
				if (_fmt_printf_f(ctx))
					return -2;
				break;
			default:
				return -1;
		}
	}

	return 0;
} /* end */
