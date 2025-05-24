/* json_parser.c - json (javascript object notation) parser implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ul/json_parser.h>


/* @def: json
* [
*   {},
*   [],
*   {
*     "": 123,
*     "": 123.123,
*     "": .123,
*     "": -12,
*     "": "",
*     "": {},
*     "": [],
*     "": true,
*     "": -123e12,
*     "": 3.14e+10
*   },
*   [ 123, "", true, false, null, {}, [ 1, 2, "", {}, 1.2 ], ],
*   [],
* ]
*/

enum {
	TOKEN_ARRAY = 1,
	TOKEN_ARRAY_END,
	TOKEN_OBJECT,
	TOKEN_OBJECT_END,
	TOKEN_VALUE,
	TOKEN_NEXT,
	TOKEN_STRING,
	TOKEN_NUMBER,
	TOKEN_NULL,
	TOKEN_TRUE,
	TOKEN_FALSE
};

static int32 _json_token(char c);
static int32 _json_string(struct json_ctx *ctx);
static int32 _json_number(struct json_ctx *ctx);
static int32 _json_null(struct json_ctx *ctx);
static int32 _json_true(struct json_ctx *ctx);
static int32 _json_false(struct json_ctx *ctx);
static int32 _json_object(struct json_ctx *ctx);
static int32 _json_array(struct json_ctx *ctx);

#undef BREAK_CHARACTER
#define BREAK_CHARACTER(x) (x == ' ' || x == '\n' || x == '\t')
#undef INVALID_CHARACTER
#define INVALID_CHARACTER(x) ((uint8)(x) < 0x20)
/* end */

/* @func: _json_token (static) - get json token
* @param1: char  # character
* @return: int32 # token value (-1: unknown token)
*/
static int32 _json_token(char c) {
	switch (c) {
		case '[':
			return TOKEN_ARRAY;
		case ']':
			return TOKEN_ARRAY_END;
		case '{':
			return TOKEN_OBJECT;
		case '}':
			return TOKEN_OBJECT_END;
		case ',':
			return TOKEN_NEXT;
		case ':':
			return TOKEN_VALUE;
		case '"':
			return TOKEN_STRING;
		case 'n':
			return TOKEN_NULL;
		case 't':
			return TOKEN_TRUE;
		case 'f':
			return TOKEN_FALSE;
		default:
			if (c == '+' || c == '-') {
				return TOKEN_NUMBER;
			} else if (c >= '0' && c <= '9') {
				return TOKEN_NUMBER;
			} else if (c == '.') {
				return TOKEN_NUMBER;
			}

			return -1;
	}

	return -1;
} /* end */

/* @func: _json_string (static) - get the length of the json string
* @param1: struct json_ctx * # json struct context
* @return: int32             # string length (-1: error)
*/
static int32 _json_string(struct json_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (c != '"')
					return -1;
				st = 1;
				break;
			case 1:
				if (c == '"') { /* end */
					ctx->str++;
					ctx->len++;
					return ctx->len - len;
				} else if (c == '\\') { /* escape */
					st = 2;
				}
				if (INVALID_CHARACTER(c)) {
					ctx->err = JSON_ERR_INVALID;
					return -1;
				}
				break;
			case 2: /* escape */
				st = 1;
				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _json_number (static) - get the length of the json number
* @param1: struct json_ctx * # json struct context
* @return: int32             # number length (-1: error)
*/
static int32 _json_number(struct json_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (c == '+' || c == '-') {
					st = 1;
					break;
				} else if (c >= '0' && c <= '9') {
					st = 1;
					break;
				} else if (c == '.') {
					st = 2;
					break;
				}
				return -1;
			case 1: /* integer */
				if (c >= '0' && c <= '9') {
					break;
				} else if (c == '.') { /* floating-point */
					st = 2;
					break;
				} else if (c == 'e') { /* exponent */
					st = 4;
					break;
				}
				return ctx->len - len;
			case 2: /* floating-point start */
				if (!(c >= '0' && c <= '9'))
					return -1;
				st = 3;
				break;
			case 3: /* floating-point */
				if (c >= '0' && c <= '9') {
					break;
				} else if (c == 'e') { /* exponent */
					st = 4;
					break;
				}
				return ctx->len - len;
			case 4: /* exponent start */
				if (c == '+' || c == '-') {
					st = 5;
					break;
				} else if (c >= '0' && c <= '9') {
					st = 5;
					break;
				}
				return -1;
			case 5: /* exponent */
				if (c >= '0' && c <= '9')
					break;
				return ctx->len - len;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _json_null (static) - get the length of the json null
* @param1: struct json_ctx * # json struct context
* @return: int32             # null length (-1: error)
*/
static int32 _json_null(struct json_ctx *ctx) {
	if (!XSYMBOL(strncmp)("null", ctx->str, 4)) {
		ctx->str += 4;
		ctx->len += 4;
		return 4;
	}

	return -1;
} /* end */

/* @func: _json_true (static) - get the length of the json true
* @param1: struct json_ctx * # json struct context
* @return: int32             # true length (-1: error)
*/
static int32 _json_true(struct json_ctx *ctx) {
	if (!XSYMBOL(strncmp)("true", ctx->str, 4)) {
		ctx->str += 4;
		ctx->len += 4;
		return 4;
	}

	return -1;
} /* end */

/* @func: _json_false (static) - get the length of the json false
* @param1: struct json_ctx * # json struct context
* @return: int32             # false length (-1: error)
*/
static int32 _json_false(struct json_ctx *ctx) {
	if (!XSYMBOL(strncmp)("false", ctx->str, 5)) {
		ctx->str += 5;
		ctx->len += 5;
		return 5;
	}

	return -1;
} /* end */

/* @func: _json_object (static) - parse json object
* @param1: struct json_ctx * # json struct context
* @return: int32             # 0: no error, -1: error, -2: call error
*/
static int32 _json_object(struct json_ctx *ctx) {
	int32 st = 0, k = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		if (BREAK_CHARACTER(c))
			continue;
		if (INVALID_CHARACTER(c)) {
			ctx->err = JSON_ERR_INVALID;
			return -1;
		}

		switch (st) {
			case 0: /* start */
				if (_json_token(c) != TOKEN_OBJECT) {
					ctx->err = JSON_ERR_OBJECT_START;
					return -1;
				}
				if (ctx->call(JSON_OBJECT_TYPE,
						NULL,
						0,
						ctx->arg))
					return -2;
				st = 1;
				break;
			case 1:
				switch (_json_token(c)) {
					case TOKEN_STRING: /* value */
						k = _json_string(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_OBJECT_VALUE;
							return -1;
						}
						if (ctx->call(JSON_VALUE_TYPE,
								ctx->str - k + 1,
								k - 2,
								ctx->arg))
							return -2;
						ctx->str--;
						ctx->len--;
						st = 2;
						break;
					case TOKEN_OBJECT_END: /* end */
						if (ctx->call_end(JSON_OBJECT_TYPE,
								ctx->arg))
							return -2;
						ctx->str++;
						ctx->len++;
						return 0;
					default:
						ctx->err = JSON_ERR_OBJECT_TOKEN;
						return -1;
				}
				break;
			case 2: /* value */
				if (_json_token(c) != TOKEN_VALUE) {
					ctx->err = JSON_ERR_OBJECT_VALUE;
					return -1;
				}
				st = 3;
				break;
			case 3:
				switch (_json_token(c)) {
					case TOKEN_STRING:
						k = _json_string(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_OBJECT_STRING;
							return -1;
						}
						if (ctx->call(JSON_STRING_TYPE,
								ctx->str - k + 1,
								k - 2,
								ctx->arg))
							return -2;
						break;
					case TOKEN_NUMBER:
						k = _json_number(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_OBJECT_NUMBER;
							return -1;
						}
						if (ctx->call(JSON_NUMBER_TYPE,
								ctx->str - k,
								k,
								ctx->arg))
							return -2;
						break;
					case TOKEN_NULL:
						k = _json_null(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_OBJECT_NULL;
							return -1;
						}
						if (ctx->call(JSON_NULL_TYPE,
								NULL,
								0,
								ctx->arg))
							return -2;
						break;
					case TOKEN_TRUE:
						k = _json_true(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_OBJECT_TRUE;
							return -1;
						}
						if (ctx->call(JSON_TRUE_TYPE,
								NULL,
								0,
								ctx->arg))
							return -2;
						break;
					case TOKEN_FALSE:
						k = _json_false(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_OBJECT_FALSE;
							return -1;
						}
						if (ctx->call(JSON_FALSE_TYPE,
								NULL,
								0,
								ctx->arg))
							return -2;
						break;
					case TOKEN_OBJECT: /* object */
						k = _json_object(ctx);
						if (k < 0)
							return k;
						break;
					case TOKEN_ARRAY: /* array */
						k = _json_array(ctx);
						if (k < 0)
							return k;
						break;
					default:
						ctx->err = JSON_ERR_OBJECT_TOKEN;
						return -1;
				}
				ctx->str--;
				ctx->len--;
				st = 4;
				break;
			case 4: /* next or end */
				switch (_json_token(c)) {
					case TOKEN_NEXT:
						st = 1;
						break;
					case TOKEN_OBJECT_END:
						if (ctx->call_end(JSON_OBJECT_TYPE,
								ctx->arg))
							return -2;
						ctx->str++;
						ctx->len++;
						return 0;
					default:
						ctx->err = JSON_ERR_OBJECT_END;
						return -1;
				}
				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _json_array (static) - parse json array
* @param1: struct json_ctx * # json struct context
* @return: int32             # 0: no error, -1: error, -2: call error
*/
static int32 _json_array(struct json_ctx *ctx) {
	int32 st = 0, k = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		if (BREAK_CHARACTER(c))
			continue;
		if (INVALID_CHARACTER(c)) {
			ctx->err = JSON_ERR_INVALID;
			return -1;
		}

		switch (st) {
			case 0: /* start */
				if (_json_token(c) != TOKEN_ARRAY) {
					ctx->err = JSON_ERR_ARRAY_START;
					return -1;
				}
				if (ctx->call(JSON_ARRAY_TYPE,
						NULL, 0,
						ctx->arg))
					return -2;
				st = 1;
				break;
			case 1:
				switch (_json_token(c)) {
					case TOKEN_STRING:
						k = _json_string(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_ARRAY_STRING;
							return -1;
						}
						if (ctx->call(JSON_STRING_TYPE,
								ctx->str - k + 1,
								k - 2,
								ctx->arg))
							return -2;
						break;
					case TOKEN_NUMBER:
						k = _json_number(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_ARRAY_NUMBER;
							return -1;
						}
						if (ctx->call(JSON_NUMBER_TYPE,
								ctx->str - k,
								k,
								ctx->arg))
							return -2;
						break;
					case TOKEN_NULL:
						k = _json_null(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_ARRAY_NULL;
							return -1;
						}
						if (ctx->call(JSON_NULL_TYPE,
								NULL,
								0,
								ctx->arg))
							return -2;
						break;
					case TOKEN_TRUE:
						k = _json_true(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_ARRAY_TRUE;
							return -1;
						}
						if (ctx->call(JSON_TRUE_TYPE,
								NULL,
								0,
								ctx->arg))
							return -2;
						break;
					case TOKEN_FALSE:
						k = _json_false(ctx);
						if (k < 0) {
							ctx->err = JSON_ERR_ARRAY_FALSE;
							return -1;
						}
						if (ctx->call(JSON_FALSE_TYPE,
								NULL,
								0,
								ctx->arg))
							return -2;
						break;
					case TOKEN_OBJECT: /* object */
						k = _json_object(ctx);
						if (k < 0)
							return k;
						break;
					case TOKEN_ARRAY: /* array */
						k = _json_array(ctx);
						if (k < 0)
							return k;
						break;
					case TOKEN_ARRAY_END: /* end */
						if (ctx->call_end(JSON_ARRAY_TYPE,
								ctx->arg))
							return -2;
						ctx->str++;
						ctx->len++;
						return 0;
					default:
						ctx->err = JSON_ERR_ARRAY_TOKEN;
						return -1;
				}
				ctx->str--;
				ctx->len--;
				st = 2;
				break;
			case 2: /* next or end */
				switch (_json_token(c)) {
					case TOKEN_NEXT:
						st = 1;
						break;
					case TOKEN_ARRAY_END:
						if (ctx->call_end(JSON_ARRAY_TYPE,
								ctx->arg))
							return -2;
						ctx->str++;
						ctx->len++;
						return 0;
					default:
						ctx->err = JSON_ERR_ARRAY_END;
						return -1;
				}
				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: json_parser - json parser
* @param1: struct json_ctx * # json struct context
* @param2: const char *      # input buffer
* @return: int32             # 0: no error, -1: error, -2: call error
*/
int32 FSYMBOL(json_parser)(struct json_ctx *ctx, const char *s) {
	ctx->str = s;
	ctx->len = 0;
	ctx->err = 0;

	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		if (BREAK_CHARACTER(c))
			continue;
		if (INVALID_CHARACTER(c)) {
			ctx->err = JSON_ERR_INVALID;
			return -1;
		}

		switch (_json_token(c)) {
			case TOKEN_OBJECT:
				return _json_object(ctx);
			case TOKEN_ARRAY:
				return _json_array(ctx);
			default:
				ctx->err = JSON_ERR_START_TOKEN;
				return -1;
		}
	}

	return -1;
} /* end */
