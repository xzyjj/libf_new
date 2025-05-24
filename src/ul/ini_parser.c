/* ini_parser.c - ini file format parser implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ul/ini_parser.h>


/* @def: ini
* [name.name] # Comments
* name =      ; Comments
*             ; Comments
* name = Str ing"
*/

static const uint8 ini_section_table[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

static const uint8 ini_key_table[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

static const uint8 ini_value_table[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
/* end */

/* @func: _ini_section (static) - get the location and length of the \
*                                 ini section
* @param1: struct ini_ctx * # ini struct context
* @param2: const char **    # location pointer
* @return: int32            # string length (-1: error)
*/
static int32 _ini_section(struct ini_ctx *ctx, const char **p) {
	int32 st = 0, len = 0;
	for (; *(ctx->str) != '\0'; ctx->str++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (c == ' ' || c == '\t')
					break;
				if (c != '[')
					return -1;

				st = 1;
				break;
			case 1: /* section name start */
				if (c == ' ' || c == '\t')
					break;
				/* invalid character */
				if (ini_section_table[(uint8)c])
					return -1;

				*p = ctx->str;
				len++;
				st = 2;
				break;
			case 2: /* section name */
				if (c == ' ' || c == '\t') {
					st = 3;
					break;
				}
				if (c == ']') { /* section end */
					st = 4;
					break;
				}

				/* invalid character */
				if (ini_section_table[(uint8)c])
					return -1;

				len++;
				break;
			case 3: /* section end */
				if (c == ' ' || c == '\t')
					break;
				if (c != ']')
					return -1;

				st = 4;
				break;
			case 4: /* end */
				if (c == '\n' || c == '#' || c == ';')
					return len;
				if (c == ' ' || c == '\t')
					break;
				return -1;
			default:
				return -1;
		}
	}
	if (st == 4)
		return len;

	return -1;
} /* end */

/* @func: _ini_key (static) - get the key location and length of the ini value
* @param1: struct ini_ctx * # ini struct context
* @param2: const char **    # location pointer
* @return: int32            # string length (-1: error)
*/
static int32 _ini_key(struct ini_ctx *ctx, const char **p) {
	int32 st = 0, len = 0;
	for (; *(ctx->str) != '\0'; ctx->str++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (c == ' ' || c == '\t')
					break;

				/* invalid character */
				if (ini_key_table[(uint8)c])
					return -1;

				*p = ctx->str;
				len++;
				st = 1;
				break;
			case 1: /* key name or end */
				if (c == '=') /* end */
					return len;
				if (c == ' ' || c == '\t') {
					st = 2;
					break;
				}

				/* invalid character */
				if (ini_key_table[(uint8)c])
					return -1;

				len++;
				break;
			case 2: /* end */
				if (c == '=')
					return len;
				if (c == ' ' || c == '\t')
					break;
				return -1;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _ini_value (static) - get the location and length of the ini value
* @param1: struct ini_ctx * # ini struct context
* @param2: const char **    # location pointer
* @return: int32            # string length (-1: error)
*/
static int32 _ini_value(struct ini_ctx *ctx, const char **p) {
	int32 st = 0;
	const char *e = NULL;
	for (; *(ctx->str) != '\0'; ctx->str++) {
		char c = *(ctx->str);
		switch (st) {
			case 0:
				if (c != '=')
					return -1;

				st = 1;
				break;
			case 1: /* start */
				if (c == ' ' || c == '\t')
					break;
				if (c == '\n' || c == '#' || c == ';')
					return 0;

				/* invalid character */
				if (ini_value_table[(uint8)c])
					return -1;

				*p = e = ctx->str;
				st = 2;
				break;
			case 2: /* value and end */
				if (c == '\n' || c == '#' || c == ';') /* end */
					return e + 1 - *p;

				if (c == ' ' || c == '\t') {
					break;
				} else {
					e = ctx->str;
				}

				/* invalid character */
				if (ini_value_table[(uint8)c])
					return -1;

				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: ini_parser - ini parser
* @param1: struct ini_ctx * # ini struct context
* @param2: const char *     # input buffer
* @return: int32            # 0: no error, -1: error, -2: call error
*/
int32 FSYMBOL(ini_parser)(struct ini_ctx *ctx, const char *s) {
	ctx->str = s;
	ctx->line = 0;
	ctx->err = 0;

	int32 st_line = 0, k = 0;
	const char *e = NULL, *p;
	for (; *(ctx->str) != '\0'; ctx->str++) {
		char c = *(ctx->str);
		if (c == ' ' || c == '\t' || c == '\r')
			continue;

		/* end location */
		if (!st_line) {
			e = XSYMBOL(strchr)(ctx->str, '\n');
			if (!e)
				e = ctx->str + XSYMBOL(strlen)(ctx->str);
			ctx->line++;
			st_line = 1;
		}
		/* skip */
		if (c == '\n' || c == '#' || c == ';') {
			ctx->str = e;
			st_line = 0;
			continue;
		}

		if (c == '[') {
			k = _ini_section(ctx, &p);
			if (k < 0)
				return -1;
			if (ctx->call(INI_SECTION_TYPE, p, k, ctx->arg))
				return -2;
		} else {
			k = _ini_key(ctx, &p);
			if (k < 0)
				return -1;
			if (ctx->call(INI_KEY_TYPE, p, k, ctx->arg))
				return -2;

			k = _ini_value(ctx, &p);
			if (k < 0)
				return -1;
			if (ctx->call(INI_VALUE_TYPE, p, k, ctx->arg))
				return -2;
		}

		ctx->str = e;
		st_line = 0;
	}

	return 0;
} /* end */
