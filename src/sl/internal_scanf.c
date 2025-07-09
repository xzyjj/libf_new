/* internal_scanf.c - string convert to format value implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdlib.h>
#include <libf/sl/internal.h>


/* @def: fmt_scanf */
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
#undef FG_SKIP
#define FG_SKIP 0x20

#undef SC_SPACE
#define SC_SPACE " \t\n"
/* end */

/* @func: _fmt_scanf_di (static) - scanf number conversion
* @param1: struct fmt_scanf_ctx * # fmt-scanf struct context
* @return: int32                  # 0: no error, <0: error
*/
int32 _fmt_scanf_di(struct fmt_scanf_ctx *ctx) {
	int32 b = 10;
	uint64L n = 0, m = 0;
	const char *s = ctx->s;

	if (ctx->flags & FG_LONG) {
		m = INT64_MAX;
	} else if (ctx->flags & FG_LONG_LONG) {
		m = INT64L_MAX;
	} else if (ctx->flags & FG_SHORT) {
		m = INT16_MAX;
	} else if (ctx->flags & FG_CHAR) {
		m = INT8_MAX;
	} else {
		m = INT32_MAX;
	}

	if (*s == '-' || *s == '+')
		s++;
	if (*s == '0') {
		s++;
		if (*s == 'X' || *s == 'x') {
			b = 16;
		} else if (*s >= '0' && *s <= '7') {
			b = 8;
		}
		if (ctx->specifiers == 'd' && b != 10)
			return -1;
	}

	n = XSYMBOL(internal_strtoull)(ctx->s, (char **)&ctx->s, &m, b);
	n = m ? n : (uint64L)-1;

	if (!(ctx->flags & FG_SKIP)) {
		if (ctx->flags & FG_LONG) {
			*(ctx->va._i64) = (int64)n;
		} else if (ctx->flags & FG_LONG_LONG) {
			*(ctx->va._i64L) = (int64L)n;
		} else if (ctx->flags & FG_SHORT) {
			*(ctx->va._i16) = (int16)n;
		} else if (ctx->flags & FG_CHAR) {
			*(ctx->va._i8) = (int8)n;
		} else {
			*(ctx->va._i32) = (int32)n;
		}
	}

	return 0;
} /* end */

/* @func: _fmt_scanf_ouxXp (static) - scanf number conversion
* @param1: struct fmt_scanf_ctx * # fmt-scanf struct context
* @return: int32                  # 0: no error, <0: error
*/
int32 _fmt_scanf_ouxXp(struct fmt_scanf_ctx *ctx) {
	int32 b = 0;
	uint64L n = 0, m = 0;

	if (ctx->flags & FG_LONG || ctx->specifiers == 'p') {
		m = UINT64_MAX;
	} else if (ctx->flags & FG_LONG_LONG) {
		m = UINT64L_MAX;
	} else if (ctx->flags & FG_SHORT) {
		m = UINT16_MAX;
	} else if (ctx->flags & FG_CHAR) {
		m = UINT8_MAX;
	} else {
		m = UINT32_MAX;
	}

	switch (ctx->specifiers) {
		case 'o':
			b = 8;
			break;
		case 'u':
			b = 10;
			break;
		case 'x': case 'X': case 'p':
			b = 16;
			break;
		default:
			return -1;
	}

	n = XSYMBOL(internal_strtoull)(ctx->s, (char **)&ctx->s, &m, b);
	n = m ? n : (uint64L)-1;

	if (!(ctx->flags & FG_SKIP)) {
		if (ctx->specifiers == 'p') {
			*(ctx->va._void) = (void *)n;
		} else if (ctx->flags & FG_LONG) {
			*(ctx->va._u64) = (uint64)n;
		} else if (ctx->flags & FG_LONG_LONG) {
			*(ctx->va._u64L) = (uint64L)n;
		} else if (ctx->flags & FG_SHORT) {
			*(ctx->va._u16) = (uint16)n;
		} else if (ctx->flags & FG_CHAR) {
			*(ctx->va._u8) = (uint8)n;
		} else {
			*(ctx->va._u32) = (uint32)n;
		}
	}

	return 0;
} /* end */

/* @func: _fmt_scanf_cs (static) - scanf char/string conversion
* @param1: struct fmt_scanf_ctx * # fmt-scanf struct context
* @return: int32                  # 0: no error, <0: error
*/
int32 _fmt_scanf_cs(struct fmt_scanf_ctx *ctx) {
	char *s = (char *)(ctx->s);

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	switch (ctx->specifiers) {
		case 'c':
			if (!(ctx->flags & FG_SKIP))
				*(ctx->va._char) = *s;
			s++;
			break;
		case 's':
			/* is-space matching */
			for (; *s != '\0'; s++) {
				if (XSYMBOL(strchr)(SC_SPACE, *s))
				 	break;
			}

			/* copy */
			if (ctx->flags & FG_SKIP)
				break;

			XSYMBOL(memcpy)(ctx->va._char, ctx->s,
				ctx->buf_size ?
					MIN(ctx->buf_size, s - ctx->s)
					: (s - ctx->s));
			ctx->va._char[ctx->buf_size ?
				MIN(ctx->buf_size, s - ctx->s)
				: (s - ctx->s)] = '\0';
			break;
		default:
			return -1;
	}
	ctx->s = s;

	return 0;
} /* end */

/* @func: _fmt_scanf_f (static) - scanf floating conversion
* @param1: struct fmt_scanf_ctx * # fmt-scanf struct context
* @return: int32                  # 0: no error, <0: error
*/
int32 _fmt_scanf_f(struct fmt_scanf_ctx *ctx) {
	float64 n = XSYMBOL(strtod)(ctx->s, (char **)&ctx->s);
	if (!(ctx->flags & FG_SKIP))
		*(ctx->va._f64) = n;

	return 0;
} /* end */

/* @func: _fmt_scanf_expr (static) - scanf character set matching
* @param1: struct fmt_scanf_ctx * # fmt-scanf struct context
* @param2: const char **          # fmt string pointer
* @return: int32                  # 0: no error, <0: error
*/
int32 _fmt_scanf_expr(struct fmt_scanf_ctx *ctx, const char **fmt) {
	char table[256], cs[256];
	XSYMBOL(memset)(table, 0, sizeof(table));
	const char *s = *fmt;

	int32 st = 0, tmp = 0, not = 0;
	for (; *s != '\0'; s++) {
		char c = *s;
		switch (st) {
			case 0: /* start */
				if (c != '[')
					return -1;
				st = 1;
				break;
			case 1: /* not, or character */
				if (c == '^') {
					not = 1;
				} else {
					table[(uint8)c] = 1;
					tmp = (uint8)c;
				}
				st = 2;
				break;
			case 2: /* character or range */
				if (c == ']') /* end */
					goto e;

				if (c == '-') { /* range */
					st = 3;
				} else {
					table[(uint8)c] = 1;
					tmp = (uint8)c;
				}
				break;
			case 3: /* range */
				if (c == ']') { /* end */
					table['-'] = 1;
					goto e;
				}

				for (; tmp <= (uint8)c; tmp++)
					table[tmp] = 1;
				st = 2;
				break;
			default:
				return -1;
		}
	}
e:
	if (*s != ']')
		return -1;
	*fmt = s;

	/* character set */
	int32 n = 0;
	for (int32 i = 1; i < 256; i++) {
		if (table[i])
			cs[n++] = (char)i;
	}
	cs[n] = '\0';

	/* character set matching */
	s = ctx->s;
	for (; *s != '\0'; s++) {
		if (not) {
			if (XSYMBOL(strchr)(cs, *s) && not)
				break;
		} else {
			if (!XSYMBOL(strchr)(cs, *s))
				break;
		}
	}

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	/* copy */
	if (!(ctx->flags & FG_SKIP)) {
		XSYMBOL(memcpy)(ctx->va._char, ctx->s,
			ctx->buf_size ?
				MIN(ctx->buf_size, s - ctx->s)
				: (s - ctx->s));
		ctx->va._char[ctx->buf_size ?
			MIN(ctx->buf_size, s - ctx->s)
			: (s - ctx->s)] = '\0';
	}
	ctx->s = s;

	return 0;
} /* end */

/* @func: internal_fmt_scanf - string convert to formatted value
* @param1: struct fmt_scanf_ctx * # fmt-scanf struct context
* @param2: const char *           # string
* @param3: const char *           # formatted string
* @param4: va_list                # variable argument list
* @return: int32                  # 0: no error, <0: error
*/
int32 XSYMBOL(internal_fmt_scanf)(struct fmt_scanf_ctx *ctx, const char *s,
		const char *fmt, va_list ap) {
	int32 n = 0;
	ctx->s = s;

	for (; *fmt != '\0'; fmt++) {
		if (*fmt == '%') {
			fmt++;
			if (*fmt == '%') {
				if (*(ctx->s) != *fmt)
					return -1;
				ctx->s++;
				continue;
			}
		} else {
			if (*fmt == ' ') {
				if (!XSYMBOL(strchr)(SC_SPACE, *(ctx->s)))
					return -1;
			} else if (*(ctx->s) != *fmt) {
				return -1;
			}
			ctx->s++;
			continue;
		}
		ctx->flags = 0;
		ctx->buf_size = 0;

		/* assignment-suppression character */
		if (*fmt == '*') {
			ctx->flags |= FG_SKIP;
			fmt++;
		}

#undef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

		/* buffer size limit */
		if (*fmt >= '0' && *fmt <= '9') {
			ctx->buf_size = XSYMBOL(strtol)(fmt,
				(char **)&fmt, 10);
			ctx->buf_size = MAX(ctx->buf_size, 0);
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

		ctx->specifiers = *fmt;
		switch (*fmt) {
			case 'n':
				return -1;
			case 'd': /* decimal */
			case 'i': /* prefix determination */
				if (ctx->flags & FG_SKIP)
					break;

				if (ctx->flags & FG_LONG) {
					ctx->va._i64 = va_arg(ap, int64 *);
				} else if (ctx->flags & FG_LONG_LONG) {
					ctx->va._i64L = va_arg(ap, int64L *);
				} else if (ctx->flags & FG_SHORT) {
					ctx->va._i16 = va_arg(ap, int16 *);
				} else if (ctx->flags & FG_CHAR) {
					ctx->va._i8 = va_arg(ap, int8 *);
				} else {
					ctx->va._i32 = va_arg(ap, int32 *);
				}
				break;
			case 'o': /* octal */
			case 'u': /* decimal */
			case 'x': /* hexadecimal */
			case 'X':
			case 'p': /* pointer */
				if (ctx->flags & FG_SKIP)
					break;

				if (*fmt == 'p') {
					ctx->va._void = va_arg(ap, void **);
				} else if (ctx->flags & FG_LONG) {
					ctx->va._u64 = va_arg(ap, uint64 *);
				} else if (ctx->flags & FG_LONG_LONG) {
					ctx->va._u64L = va_arg(ap, uint64L *);
				} else if (ctx->flags & FG_SHORT) {
					ctx->va._u16 = va_arg(ap, uint16 *);
				} else if (ctx->flags & FG_CHAR) {
					ctx->va._u8 = va_arg(ap, uint8 *);
				} else {
					ctx->va._u32 = va_arg(ap, uint32 *);
				}
				break;
			case 'c': /* character */
			case 's': /* string */
			case '[': /* string expr */
				if (ctx->flags & FG_SKIP)
					break;

				ctx->va._char = va_arg(ap, char *);
				break;
			case 'e': /* floating-point */
			case 'E':
			case 'f': case 'F': case 'g':
			case 'G': case 'a': case 'A':
				if (ctx->flags & FG_SKIP)
					break;

				ctx->va._f64 = va_arg(ap, float64 *);
				break;
			default:
				return -1;
		}

		switch (ctx->specifiers) {
			case 'n':
				return -1;
			case 'd': /* decimal */
			case 'i': /* prefix determination */
				if (_fmt_scanf_di(ctx))
					return n;
				break;
			case 'o': /* octal */
			case 'u': /* decimal */
			case 'x': /* hexadecimal */
			case 'X':
			case 'p': /* pointer */
				if (_fmt_scanf_ouxXp(ctx))
					return n;
				break;
			case 'c': /* character */
			case 's': /* string */
				if (_fmt_scanf_cs(ctx))
					return n;
				break;
			case '[': /* string expr */
				if (_fmt_scanf_expr(ctx, &fmt))
					return n;
				break;
			case 'e': /* floating-point */
			case 'E':
			case 'f': case 'F': case 'g':
			case 'G': case 'a': case 'A':
				if (_fmt_scanf_f(ctx))
					return n;
				break;
			default:
				return -1;
		}
		n++;
	}

	return n;
} /* end */
