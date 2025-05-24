/* xml_parser.c - xml (extensible markup language) parser implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ul/xml_parser.h>


/* @def: xml
* <?xxx xxx="xxx"?>
* <? xxx xxx="xxx" ?>
* <?xml version="1.0" encode="utf-8" ?>
* <!DOCTYPE>
* <!DOCTYPE name>
* <!DOCTYPE name name>
* <!DOCTYPE name name "string">
* <!-- Comments -->
* <element attribute="xxx">String <!-- Comments --> </element>
* < element >
*   String
*   < element-2 attribute="xxx"> String < /element-2>String
* </element>
* <element attribute="xxx" />
* <![CDATA[......]]>
*/

enum {
	TOKEN_STATEMENT = 1,
	TOKEN_COMMENT,
	TOKEN_DOCTYPE,
	TOKEN_CDATA,
	TOKEN_ELEMENT,
	TOKEN_ELEMENT_END
};

static const uint8 xml_name_table[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
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

#undef BREAK_CHARACTER
#define BREAK_CHARACTER(x) (x == ' ' || x == '\n' || x == '\t')
#undef INVALID_CHARACTER
#define INVALID_CHARACTER(x) ((uint8)(x) < 0x20)
/* end */

/* @func: _xml_token (static) - get xml token
* @param1: const char * # input string
* @return: int32        # token value (-1: error)
*/
static int32 _xml_token(const char *s) {
	const char *p;
	char c = 0;
	if (*s != '<')
		return -1;

	/* processing instruction */
	if (!XSYMBOL(strncmp)("<?", s, 2))
		return TOKEN_STATEMENT;

	/* comment */
	if (!XSYMBOL(strncmp)("<!--", s, 4))
		return TOKEN_COMMENT;

	/* document type */
	if (!XSYMBOL(strncmp)("<!DOCTYPE", s, 9)) {
		c = *(s + 9);
		if (c == '>' || BREAK_CHARACTER(c))
			return TOKEN_DOCTYPE;
		return -1;
	}

	/* CDATA block */
	if (!XSYMBOL(strncmp)("<![CDATA[", s, 9))
		return TOKEN_CDATA;

	/* end-element */
	p = s;
	c = *p++;
	while (c) {
		c = *p++;
		if (BREAK_CHARACTER(c))
			continue;
		if (c != '/')
			break;
		return TOKEN_ELEMENT_END;
	}

	/* element */
	p = s;
	c = *p++;
	while (c) {
		c = *p++;
		if (BREAK_CHARACTER(c))
			return TOKEN_ELEMENT;
		if (xml_name_table[(uint8)c])
			break;
		return TOKEN_ELEMENT;
	}

	return -1;
} /* end */

/* @func: _xml_string (static) - get the length of the xml string
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # string length (-1: error)
*/
static int32 _xml_string(struct xml_ctx *ctx) {
	char cc = 0;
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (c != '"' && c != '\'')
					return -1;
				cc = c;
				st = 1;
				break;
			case 1:
				if (c == cc) { /* end */
					ctx->str++;
					ctx->len++;
					return ctx->len - len;
				} else if (c == '\\') { /* escape */
					st = 2;
				}
				if (INVALID_CHARACTER(c))
					return -1;
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

/* @func: _xml_comment (static) - get the length of the xml comment string
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # string length (-1: error)
*/
static int32 _xml_comment(struct xml_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (XSYMBOL(strncmp)("<!--", ctx->str, 4))
					return -1;
				ctx->str += 3;
				ctx->len += 3;
				st = 1;
				break;
			case 1: /* end */
				if (c != '-')
					break;
				if (XSYMBOL(strncmp)("-->", ctx->str, 3))
					break;
				ctx->str += 3;
				ctx->len += 3;
				return ctx->len - len;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_cdata (static) - get the length of the xml character data
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # string length (-1: error)
*/
static int32 _xml_cdata(struct xml_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (XSYMBOL(strncmp)("<![CDATA[", ctx->str, 9))
					return -1;
				ctx->str += 8;
				ctx->len += 8;
				st = 1;
				break;
			case 1: /* end */
				if (c != ']')
					break;
				if (XSYMBOL(strncmp)("]]>", ctx->str, 3))
					break;
				ctx->str += 3;
				ctx->len += 3;
				return ctx->len - len;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_statement_attr (static) - parsing xml statement attributes
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # 0: no error, -1: error, -2: call error
*/
static int32 _xml_statement_attr(struct xml_ctx *ctx) {
	int32 st = 0, k = 0, len = 0;
	const char *e = NULL;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* name start or end */
				if (BREAK_CHARACTER(c))
					break;
				if (!XSYMBOL(strncmp)("?>", ctx->str, 2)) /* end */
					return 0;

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_STATEMENT_ATTR_CHAR;
					return -1;
				}
				switch (c) {
					case '-':
					case '.':
					case ':':
					case '0': case '1': case '2': case '3':
					case '4': case '5': case '6': case '7':
					case '8': case '9':
						ctx->err = XML_ERR_STATEMENT_ATTR_CHAR;
						return -1;
					default:
						break;
				}

				e = ctx->str;
				len++;
				st = 1;
				break;
			case 1: /* name */
				if (c == '=') { /* value */
					if (ctx->call(XML_STATEMENT_ATTR_NAME,
							e,
							len,
							ctx->arg))
						return -2;
					st = 2;
					break;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_STATEMENT_ATTR_CHAR;
					return -1;
				}

				len++;
				break;
			case 2: /* value and next */
				k = _xml_string(ctx);
				if (k < 0) {
					ctx->err = XML_ERR_STATEMENT_ATTR_VALUE;
					return -1;
				}
				if (ctx->call(XML_STATEMENT_ATTR_VALUE,
						ctx->str - k + 1,
						k - 2,
						ctx->arg))
					return -2;

				ctx->str--;
				ctx->len--;
				len = 0;
				st = 0;
				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_statement (static) - parsing xml statement
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # 0: no error, -1: error, -2: call error
*/
static int32 _xml_statement(struct xml_ctx *ctx) {
	int32 st = 0, k = 0, len = 0;
	const char *e = NULL;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start or end */
				if (XSYMBOL(strncmp)("<?", ctx->str, 2)) {
					ctx->err = XML_ERR_STATEMENT_START;
					return -1;
				}

				ctx->str++;
				ctx->len++;
				st = 1;
				break;
			case 1: /* name start or end */
				if (BREAK_CHARACTER(c))
					break;
				if (!XSYMBOL(strncmp)("?>", ctx->str, 2)) { /* end */
					ctx->str += 2;
					ctx->len += 2;
					return 0;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_STATEMENT_CHAR;
					return -1;
				}
				switch (c) {
					case '-':
					case '.':
					case ':':
					case '0': case '1': case '2': case '3':
					case '4': case '5': case '6': case '7':
					case '8': case '9':
						ctx->err = XML_ERR_STATEMENT_CHAR;
						return -1;
					default:
						break;
				}

				e = ctx->str;
				len++;
				st = 2;
				break;
			case 2: /* name or end */
				if (BREAK_CHARACTER(c)) { /* attribute */
					if (ctx->call(XML_STATEMENT_START,
							e,
							len,
							ctx->arg))
						return -2;
					st = 3;
					break;
				} else if (!XSYMBOL(strncmp)("?>", ctx->str, 2)) { /* end */
					if (ctx->call(XML_STATEMENT_START,
							e,
							len,
							ctx->arg))
						return -2;
					ctx->str += 2;
					ctx->len += 2;
					return 0;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_STATEMENT_CHAR;
					return -1;
				}

				len++;
				break;
			case 3: /* attribute or end */
				k = _xml_statement_attr(ctx); /* attribute */
				if (k < 0)
					return k;

				ctx->str += 2;
				ctx->len += 2;
				return 0;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_doctype (static) - parsing xml doctype
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # 0: no error, -1: error, -2: call error
*/
static int32 _xml_doctype(struct xml_ctx *ctx) {
	/*
	* <!DOCTYPE>
	*   start --> end
	*
	* <!DOCTYPE name>
	*   start --> name --> end
	*     name --> end
	*
	* <!DOCTYPE name name>
	*   start --> name --> end
	*     name --> next +--> end
	*       `<----------'
	*
	* <!DOCTYPE name "string">
	*   start --> name --> end
	*     name --> next --> string --> end
	*
	* <!DOCTYPE name "string" "string">
	*   start --> name --> end
	*     name --> next --> string --> next +--> end
	*                           `<----------'
	*/

	int32 st = 0, k = 0, len = 0;
	const char *e = NULL;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start or end */
				if (XSYMBOL(strncmp)("<!DOCTYPE", ctx->str, 9)) {
					ctx->err = XML_ERR_DOCTYPE_START;
					return -1;
				}
				if (ctx->call(XML_DOCTYPE_START,
						NULL,
						0,
						ctx->arg))
					return -2;

				ctx->str += 9;
				ctx->len += 9;
				c = *(ctx->str);
				if (c == '>') { /* end */
					ctx->str++;
					ctx->len++;
					return 0;
				}
				if (!BREAK_CHARACTER(c)) {
					ctx->err = XML_ERR_DOCTYPE_START;
					return -1;
				}
				st = 1;
				break;
			case 1: /* name start, string or end */
				if (BREAK_CHARACTER(c))
					break;
				if (c == '>') { /* end */
					ctx->str++;
					ctx->len++;
					return 0;
				} else if (c == '"' || c == '\'') { /* string */
					k = _xml_string(ctx);
					if (k < 0) {
						ctx->err = XML_ERR_DOCTYPE_STRING;
						return -1;
					}
					if (ctx->call(XML_DOCTYPE_STRING,
							ctx->str - k + 1,
							k - 2,
							ctx->arg))
						return -2;
					ctx->str--;
					ctx->len--;
					st = 3;
					break;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_DOCTYPE_CHAR;
					return -1;
				}
				switch (c) {
					case '-':
					case '.':
					case ':':
					case '0': case '1': case '2': case '3':
					case '4': case '5': case '6': case '7':
					case '8': case '9':
						ctx->err = XML_ERR_DOCTYPE_CHAR;
						return -1;
					default:
						break;
				}

				e = ctx->str;
				len++;
				st = 2;
				break;
			case 2: /* name */
				if (BREAK_CHARACTER(c)) { /* next */
					if (ctx->call(XML_DOCTYPE_NAME,
							e,
							len,
							ctx->arg))
						return -2;
					st = 3;
					break;
				} else if (c == '>') { /* end */
					if (ctx->call(XML_DOCTYPE_NAME,
							e,
							len,
							ctx->arg))
						return -2;
					ctx->str++;
					ctx->len++;
					return 0;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_DOCTYPE_CHAR;
					return -1;
				}

				len++;
				break;
			case 3: /* next or end */
				if (BREAK_CHARACTER(c))
					break;
				if (c == '>') { /* end */
					ctx->str++;
					ctx->len++;
					return 0;
				} /* next */

				ctx->str--;
				ctx->len--;
				len = 0;
				st = 1;
				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_element_attr (static) - parsing xml element attributes
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # 0: no error, -1: error, -2: call error
*/
static int32 _xml_element_attr(struct xml_ctx *ctx) {
	int32 st = 0, k = 0, len = 0;
	const char *e = NULL;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* name start or end */
				if (BREAK_CHARACTER(c))
					break;
				if (c == '/' || c == '>') /* end */
					return 0;

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_ELEMENT_ATTR_CHAR;
					return -1;
				}
				switch (c) {
					case '-':
					case '.':
					case ':':
					case '0': case '1': case '2': case '3':
					case '4': case '5': case '6': case '7':
					case '8': case '9':
						ctx->err = XML_ERR_ELEMENT_ATTR_CHAR;
						return -1;
					default:
						break;
				}

				e = ctx->str;
				len++;
				st = 1;
				break;
			case 1: /* name */
				if (c == '=') { /* value */
					if (ctx->call(XML_ELEMENT_ATTR_NAME,
							e,
							len,
							ctx->arg))
						return -2;
					st = 2;
					break;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_ELEMENT_ATTR_CHAR;
					return -1;
				}

				len++;
				break;
			case 2: /* value and next */
				k = _xml_string(ctx);
				if (k < 0) {
					ctx->err = XML_ERR_ELEMENT_ATTR_VALUE;
					return -1;
				}
				if (ctx->call(XML_ELEMENT_ATTR_VALUE,
						ctx->str - k + 1,
						k - 2,
						ctx->arg))
					return -2;

				ctx->str--;
				ctx->len--;
				len = 0;
				st = 0;
				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_element_start (static) - parsing xml element
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # 0: no error, 1: empty element,  -1: error, \
*                             -2: call error
*/
static int32 _xml_element_start(struct xml_ctx *ctx) {
	/*
	* <element> string </element>
	*   element --> end
	*     string
	*   end-element
	*
	* <element attr="string"> string </element>
	*   element --> attr --> end
	*     attr --> = --> string +--> end
	*       `<------------------'
	*     string
	*   end-element
	*
	* <element />
	*   element --> empty end
	*
	* <element attr="string" />
	*   element --> attr --> empty end
	*     attr --> = --> string +--> end
	*       `<------------------'
	*/

	int32 st = 0, k = 0, len = 0;
	const char *e = NULL;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (c != '<') {
					ctx->err = XML_ERR_ELEMENT_START;
					return -1;
				}
				st = 1;
				break;
			case 1: /* name start */
				if (BREAK_CHARACTER(c))
					break;

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_ELEMENT_CHAR;
					return -1;
				}
				switch (c) {
					case '-':
					case '.':
					case ':':
					case '0': case '1': case '2': case '3':
					case '4': case '5': case '6': case '7':
					case '8': case '9':
						ctx->err = XML_ERR_ELEMENT_CHAR;
						return -1;
					default:
						break;
				}

				e = ctx->str;
				len++;
				st = 2;
				break;
			case 2: /* name or end */
				if (BREAK_CHARACTER(c)) { /* attribute */
					if (ctx->call(XML_ELEMENT_START,
							e,
							len,
							ctx->arg))
						return -2;
					st = 3;
					break;
				} else if (c == '>') { /* end */
					if (ctx->call(XML_ELEMENT_START,
							e,
							len,
							ctx->arg))
						return -2;
					ctx->str++;
					ctx->len++;
					return 0;
				} else if (c == '/') { /* empty */
					if (ctx->call(XML_ELEMENT_START,
							e,
							len,
							ctx->arg))
						return -2;
					st = 4;
					break;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_ELEMENT_CHAR;
					return -1;
				}

				len++;
				break;
			case 3: /* attribute or end */
				k = _xml_element_attr(ctx); /* attribute */
				if (k < 0)
					return k;
				if (*(ctx->str) == '/') { /* empty */
					st = 4;
					break;
				} /* end */

				ctx->str++;
				ctx->len++;
				return 0;
			case 4: /* empty element end */
				if (BREAK_CHARACTER(c))
					break;
				if (c != '>') {
					ctx->err = XML_ERR_ELEMENT_EMPTY_END;
					return -1;
				}

				if (ctx->call_end(XML_ELEMENT_EMPTY_END,
						NULL,
						0,
						ctx->arg))
					return -2;
				ctx->str++;
				ctx->len++;
				return 1;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_element_end (static) - parsing xml end-element
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # 0: no error, -1: error, -2: call error
*/
static int32 _xml_element_end(struct xml_ctx *ctx) {
	int32 st = 0, len = 0;
	const char *e = NULL;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				if (c != '<') {
					ctx->err = XML_ERR_END_ELEMENT_START;
					return -1;
				}
				st = 1;
				break;
			case 1:
				if (BREAK_CHARACTER(c))
					break;
				if (c != '/') {
					ctx->err = XML_ERR_END_ELEMENT_START;
					return -1;
				}
				st = 2;
				break;
			case 2: /* name start */
				if (BREAK_CHARACTER(c))
					break;

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_END_ELEMENT_CHAR;
					return -1;
				}
				switch (c) {
					case '-':
					case '.':
					case ':':
					case '0': case '1': case '2': case '3':
					case '4': case '5': case '6': case '7':
					case '8': case '9':
						ctx->err = XML_ERR_END_ELEMENT_CHAR;
						return -1;
					default:
						break;
				}

				e = ctx->str;
				len++;
				st = 3;
				break;
			case 3: /* name or end */
				if (BREAK_CHARACTER(c)) {
					if (ctx->call_end(XML_ELEMENT_END,
							e,
							len,
							ctx->arg))
						return -2;
					st = 4;
					break;
				} else if (c == '>') { /* end */
					if (ctx->call_end(XML_ELEMENT_END,
							e,
							len,
							ctx->arg))
						return -2;
					ctx->str++;
					ctx->len++;
					return 0;
				}

				/* invalid character */
				if (xml_name_table[(uint8)c]) {
					ctx->err = XML_ERR_END_ELEMENT_CHAR;
					return -1;
				}

				len++;
				break;
			case 4: /* end */
				if (BREAK_CHARACTER(c))
					break;

				/* invalid character */
				if (c != '>') {
					ctx->err = XML_ERR_END_ELEMENT_CHAR;
					return -1;
				} /* end */

				ctx->str++;
				ctx->len++;
				return 0;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: _xml_element (static) - parsing xml element
* @param1: struct xml_ctx * # xml struct context
* @return: int32            # 0: no error, -1: error, -2: call error
*/
static int32 _xml_element(struct xml_ctx *ctx) {
	int32 st = 0, k = 0, len = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* start */
				k = _xml_element_start(ctx);
				if (k < 0)
					return k;
				if (k == 1) /* empty */
					return 0;
				ctx->str--;
				ctx->len--;
				st = 1;
				break;
			case 1: /* element */
				if (c == '<') { /* embed */
					if (ctx->call(XML_ELEMENT_STRING,
							ctx->str - len,
							len,
							ctx->arg))
						return -2;

					switch (_xml_token(ctx->str)) {
						case TOKEN_ELEMENT: /* embed element */
							k = _xml_element(ctx);
							if (k < 0)
								return k;
							ctx->str--;
							ctx->len--;
							break;
						case TOKEN_ELEMENT_END: /* end */
							k = _xml_element_end(ctx);
							if (k < 0)
								return k;
							return 0;
						case TOKEN_COMMENT: /* comment */
							k = _xml_comment(ctx);
							if (k < 0) {
								ctx->err = XML_ERR_COMMENT;
								return k;
							}
							if (ctx->call(XML_COMMENT,
									ctx->str - k,
									k,
									ctx->arg))
								return -2;
							ctx->str--;
							ctx->len--;
							break;
						case TOKEN_CDATA: /* character data */
							k = _xml_cdata(ctx);
							if (k < 0) {
								ctx->err = XML_ERR_CDATA;
								return k;
							}
							if (ctx->call(XML_CDATA,
									ctx->str - k,
									k,
									ctx->arg))
								return -2;
							ctx->str--;
							ctx->len--;
							break;
						default:
							ctx->err = XML_ERR_TOKEN;
							return -1;
					}

					len = 0;
					break;
				}

				len++;
				break;
			default:
				return -1;
		}
	}

	return -1;
} /* end */

/* @func: xml_parser - xml parser
* @param1: struct xml_ctx * # xml struct context
* @param2: const char *     # input buffer
* @return: int32            # 0: no error, -1: error, -2: call error
*/
int32 FSYMBOL(xml_parser)(struct xml_ctx *ctx, const char *s) {
	ctx->str = s;
	ctx->len = 0;
	ctx->err = 0;

	int32 k = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		if (BREAK_CHARACTER(c))
			continue;
		if (INVALID_CHARACTER(c))
			return -1;

		switch (_xml_token(ctx->str)) {
			case TOKEN_STATEMENT:
				k = _xml_statement(ctx);
				if (k < 0)
					return k;
				ctx->str--;
				ctx->len--;
				break;
			case TOKEN_COMMENT:
				k = _xml_comment(ctx);
				if (k < 0) {
					ctx->err = XML_ERR_COMMENT;
					return k;
				}
				if (ctx->call(XML_COMMENT,
						ctx->str - k,
						k,
						ctx->arg))
					return -2;
				ctx->str--;
				ctx->len--;
				break;
			case TOKEN_DOCTYPE:
				k = _xml_doctype(ctx);
				if (k < 0)
					return k;
				ctx->str--;
				ctx->len--;
				break;
			case TOKEN_CDATA:
				k = _xml_cdata(ctx);
				if (k < 0) {
					ctx->err = XML_ERR_CDATA;
					return k;
				}
				if (ctx->call(XML_CDATA,
						ctx->str - k,
						k,
						ctx->arg))
					return -2;
				ctx->str--;
				ctx->len--;
				break;
			case TOKEN_ELEMENT:
				k = _xml_element(ctx);
				if (k < 0)
					return k;
				ctx->str--;
				ctx->len--;
				break;
			default:
				ctx->err = XML_ERR_TOKEN;
				return -1;
		}
	}

	return 0;
} /* end */
