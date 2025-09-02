#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


//#define printf(...) ((void)0)

enum {
	TOKEN_NAME = 1,         /* "\b[A-Za-z_]\|[A-Za-z_][A-za-z0-9_]\+\b" */
	TOKEN_ADD,              /* "+"          */
	TOKEN_SUB,              /* "-"          */
	TOKEN_MUL,              /* "*"          */
	TOKEN_DIV,              /* "/"          */
	TOKEN_MOD,              /* "%"          */
	TOKEN_ADD_ADD,          /* "++"         */
	TOKEN_SUB_SUB,          /* "--"         */
	TOKEN_AND,              /* "&"          */
	TOKEN_OR,               /* "|"          */
	TOKEN_XOR,              /* "^"          */
	TOKEN_NEG,              /* "~"          */
	TOKEN_L_SHIFT,          /* "<<"         */
	TOKEN_R_SHIFT,          /* ">>"         */
	TOKEN_OP_NOT,           /* "!"          */
	TOKEN_OP_AND,           /* "&&"         */
	TOKEN_OP_OR,            /* "||"         */
	TOKEN_OP_LT,            /* "<"          */
	TOKEN_OP_GT,            /* ">"          */
	TOKEN_OP_EQ,            /* "=="         */
	TOKEN_OP_NE,            /* "!="         */
	TOKEN_OP_LE,            /* "<="         */
	TOKEN_OP_GE,            /* ">="         */
	TOKEN_ASSIGN,           /* "="          */
	TOKEN_ADD_ASSIGN,       /* "+="         */
	TOKEN_SUB_ASSIGN,       /* "-="         */
	TOKEN_MUL_ASSIGN,       /* "*="         */
	TOKEN_DIV_ASSIGN,       /* "/="         */
	TOKEN_MOD_ASSIGN,       /* "%="         */
	TOKEN_AND_ASSIGN,       /* "&="         */
	TOKEN_OR_ASSIGN,        /* "|="         */
	TOKEN_XOR_ASSIGN,       /* "^="         */
	TOKEN_L_SHIFT_ASSIGN,   /* "<<="        */
	TOKEN_R_SHIFT_ASSIGN,   /* ">>="        */
	TOKEN_A_DOT,            /* "."          */
	TOKEN_A_ANY,            /* "..."        */
	TOKEN_A_PTR,            /* "->"         */
	TOKEN_A_MARK,           /* "?"          */
	TOKEN_A_COLON,          /* ":"          */
	TOKEN_A_COMMA,          /* ","          */
	TOKEN_A_SEMI,           /* ";"          */
	TOKEN_A_SIGN,           /* "#" | "%:"   */
	TOKEN_A_L_BRAK,         /* "("          */
	TOKEN_A_R_BRAK,         /* ")"          */
	TOKEN_A_L_SQBRAK,       /* "{" | "<%"   */
	TOKEN_A_R_SQBRAK,       /* "}" | "%>"   */
	TOKEN_A_L_CUBRAK,       /* "[" | "<:"   */
	TOKEN_A_R_CUBRAK,       /* "]" | ":>"   */
	TOKEN_A_STRING,         /* "<...>"      */
	TOKEN_A_CHAR,           /* "<..>"       */
	TOKEN_A_OCTAL,          /* "\b0[0-7]\+[uU]\?[lL]\?[lL]\?\b" */
	TOKEN_A_DECIMAL,        /* "\b[0-9]\+[uU]\?[lL]\?[lL]\?\b"  */
	TOKEN_A_HEX,            /* "\b0x[A-Fa-f0-9]\+[uU]\?[lL]\?[lL]\?\b" */
	TOKEN_A_FLOAT,          /* "\b[0-9]\+\?\.\?[0-9]\+\?e[+-]\?[0-9]\+\b"
				 * 0.1[Ff], 0.1e[+-]10[Ff], 1.[Ff]  */
	TOKEN_A_FLOAT_HEX,      /* "\b0x[A-Fa-f0-9]\+\?\.\?[A-Fa-f0-9]\+\?p[+-]\?[0-9]\+\b"
				 * 0x1.p[+-]1[Ff], 0x1p[+-][Ff], invented suffixe 0x1<OP> */
	TOKEN_KEYWORD_AUTO,     /* "auto"       */
	TOKEN_KEYWORD_BREAK,    /* "break"      */
	TOKEN_KEYWORD_CASE,     /* "case"       */
	TOKEN_KEYWORD_CHAR,     /* "char"       */
	TOKEN_KEYWORD_CONST,    /* "const"      */
	TOKEN_KEYWORD_CONTINUE, /* "continue"   */
	TOKEN_KEYWORD_DEFAULT,  /* "default"    */
	TOKEN_KEYWORD_DO,       /* "do"         */
	TOKEN_KEYWORD_DOUBLE,   /* "double"     */
	TOKEN_KEYWORD_ELSE,     /* "else"       */
	TOKEN_KEYWORD_ENUM,     /* "enum"       */
	TOKEN_KEYWORD_EXTERN,   /* "extern"     */
	TOKEN_KEYWORD_FLOAT,    /* "float"      */
	TOKEN_KEYWORD_FOR,      /* "for"        */
	TOKEN_KEYWORD_GOTO,     /* "goto"       */
	TOKEN_KEYWORD_IF,       /* "if"         */
	TOKEN_KEYWORD_INLINE,   /* "inline"     */
	TOKEN_KEYWORD_INT,      /* "int"        */
	TOKEN_KEYWORD_LONG,     /* "long"       */
	TOKEN_KEYWORD_REGISTER, /* "register"   */
	TOKEN_KEYWORD_RESTRICT, /* "restrict"   */
	TOKEN_KEYWORD_RETURN,   /* "return"     */
	TOKEN_KEYWORD_SHORT,    /* "short"      */
	TOKEN_KEYWORD_SIGNED,   /* "signed"     */
	TOKEN_KEYWORD_SIZEOF,   /* "sizeof"     */
	TOKEN_KEYWORD_STATIC,   /* "static"     */
	TOKEN_KEYWORD_STRUCT,   /* "struct"     */
	TOKEN_KEYWORD_SWITCH,   /* "switch"     */
	TOKEN_KEYWORD_TYPEDEF,  /* "typedef"    */
	TOKEN_KEYWORD_UNION,    /* "union"      */
	TOKEN_KEYWORD_UNSIGNED, /* "unsigned"   */
	TOKEN_KEYWORD_VOID,     /* "void"       */
	TOKEN_KEYWORD_VOLATILE, /* "volatile"   */
	TOKEN_KEYWORD_WHILE,    /* "while"      */
	TOKEN_KEYWORD_BOOL,     /* "_Bool"      */
	TOKEN_KEYWORD_COMPLEX,  /* "_Complex"   */
	TOKEN_KEYWORD_IMAGINARY /* "_Imaginary" */
};

int32 lex_print(int32 tok) {
	switch (tok) {
		case TOKEN_NAME:
			printf("TOKEN_NAME\n");
			break;
		case TOKEN_ADD:
			printf("TOKEN_ADD\n");
			break;
		case TOKEN_SUB:
			printf("TOKEN_SUB\n");
			break;
		case TOKEN_MUL:
			printf("TOKEN_MUL\n");
			break;
		case TOKEN_DIV:
			printf("TOKEN_DIV\n");
			break;
		case TOKEN_MOD:
			printf("TOKEN_MOD\n");
			break;
		case TOKEN_ADD_ADD:
			printf("TOKEN_ADD_ADD\n");
			break;
		case TOKEN_SUB_SUB:
			printf("TOKEN_SUB_SUB\n");
			break;
		case TOKEN_AND:
			printf("TOKEN_AND\n");
			break;
		case TOKEN_OR:
			printf("TOKEN_OR\n");
			break;
		case TOKEN_XOR:
			printf("TOKEN_XOR\n");
			break;
		case TOKEN_NEG:
			printf("TOKEN_NEG\n");
			break;
		case TOKEN_L_SHIFT:
			printf("TOKEN_L_SHIFT\n");
			break;
		case TOKEN_R_SHIFT:
			printf("TOKEN_R_SHIFT\n");
			break;
		case TOKEN_OP_NOT:
			printf("TOKEN_OP_NOT\n");
			break;
		case TOKEN_OP_AND:
			printf("TOKEN_OP_AND\n");
			break;
		case TOKEN_OP_OR:
			printf("TOKEN_OP_OR\n");
			break;
		case TOKEN_OP_LT:
			printf("TOKEN_OP_LT\n");
			break;
		case TOKEN_OP_GT:
			printf("TOKEN_OP_GT\n");
			break;
		case TOKEN_OP_EQ:
			printf("TOKEN_OP_EQ\n");
			break;
		case TOKEN_OP_NE:
			printf("TOKEN_OP_NE\n");
			break;
		case TOKEN_OP_LE:
			printf("TOKEN_OP_LE\n");
			break;
		case TOKEN_OP_GE:
			printf("TOKEN_OP_GE\n");
			break;
		case TOKEN_ASSIGN:
			printf("TOKEN_ASSIGN\n");
			break;
		case TOKEN_ADD_ASSIGN:
			printf("TOKEN_ADD_ASSIGN\n");
			break;
		case TOKEN_SUB_ASSIGN:
			printf("TOKEN_SUB_ASSIGN\n");
			break;
		case TOKEN_MUL_ASSIGN:
			printf("TOKEN_MUL_ASSIGN\n");
			break;
		case TOKEN_DIV_ASSIGN:
			printf("TOKEN_DIV_ASSIGN\n");
			break;
		case TOKEN_MOD_ASSIGN:
			printf("TOKEN_MOD_ASSIGN\n");
			break;
		case TOKEN_AND_ASSIGN:
			printf("TOKEN_AND_ASSIGN\n");
			break;
		case TOKEN_OR_ASSIGN:
			printf("TOKEN_OR_ASSIGN\n");
			break;
		case TOKEN_XOR_ASSIGN:
			printf("TOKEN_XOR_ASSIGN\n");
			break;
		case TOKEN_L_SHIFT_ASSIGN:
			printf("TOKEN_L_SHIFT_ASSIGN\n");
			break;
		case TOKEN_R_SHIFT_ASSIGN:
			printf("TOKEN_R_SHIFT_ASSIGN\n");
			break;
		case TOKEN_A_DOT:
			printf("TOKEN_A_DOT\n");
			break;
		case TOKEN_A_ANY:
			printf("TOKEN_A_ANY\n");
			break;
		case TOKEN_A_PTR:
			printf("TOKEN_A_PTR\n");
			break;
		case TOKEN_A_MARK:
			printf("TOKEN_A_MARK\n");
			break;
		case TOKEN_A_COLON:
			printf("TOKEN_A_COLON\n");
			break;
		case TOKEN_A_COMMA:
			printf("TOKEN_A_COMMA\n");
			break;
		case TOKEN_A_SEMI:
			printf("TOKEN_A_SEMI\n");
			break;
		case TOKEN_A_SIGN:
			printf("TOKEN_A_SIGN\n");
			break;
		case TOKEN_A_L_BRAK:
			printf("TOKEN_A_L_BRAK\n");
			break;
		case TOKEN_A_R_BRAK:
			printf("TOKEN_A_R_BRAK\n");
			break;
		case TOKEN_A_L_SQBRAK:
			printf("TOKEN_A_L_SQBRAK\n");
			break;
		case TOKEN_A_R_SQBRAK:
			printf("TOKEN_A_R_SQBRAK\n");
			break;
		case TOKEN_A_L_CUBRAK:
			printf("TOKEN_A_L_CUBRAK\n");
			break;
		case TOKEN_A_R_CUBRAK:
			printf("TOKEN_A_R_CUBRAK\n");
			break;
		case TOKEN_A_STRING:
			printf("TOKEN_A_STRING\n");
			break;
		case TOKEN_A_CHAR:
			printf("TOKEN_A_CHAR\n");
			break;
		case TOKEN_A_OCTAL:
			printf("TOKEN_A_OCTAL\n");
			break;
		case TOKEN_A_DECIMAL:
			printf("TOKEN_A_DECIMAL\n");
			break;
		case TOKEN_A_HEX:
			printf("TOKEN_A_HEX\n");
			break;
		case TOKEN_A_FLOAT:
			printf("TOKEN_A_FLOAT\n");
			break;
		case TOKEN_A_FLOAT_HEX:
			printf("TOKEN_A_FLOAT_HEX\n");
			break;
		case TOKEN_KEYWORD_AUTO:
			printf("TOKEN_KEYWORD_AUTO\n");
			break;
		case TOKEN_KEYWORD_BREAK:
			printf("TOKEN_KEYWORD_BREAK\n");
			break;
		case TOKEN_KEYWORD_CASE:
			printf("TOKEN_KEYWORD_CASE\n");
			break;
		case TOKEN_KEYWORD_CHAR:
			printf("TOKEN_KEYWORD_CHAR\n");
			break;
		case TOKEN_KEYWORD_CONST:
			printf("TOKEN_KEYWORD_CONST\n");
			break;
		case TOKEN_KEYWORD_CONTINUE:
			printf("TOKEN_KEYWORD_CONTINUE\n");
			break;
		case TOKEN_KEYWORD_DEFAULT:
			printf("TOKEN_KEYWORD_DEFAULT\n");
			break;
		case TOKEN_KEYWORD_DO:
			printf("TOKEN_KEYWORD_DO\n");
			break;
		case TOKEN_KEYWORD_DOUBLE:
			printf("TOKEN_KEYWORD_DOUBLE\n");
			break;
		case TOKEN_KEYWORD_ELSE:
			printf("TOKEN_KEYWORD_ELSE\n");
			break;
		case TOKEN_KEYWORD_ENUM:
			printf("TOKEN_KEYWORD_ENUM\n");
			break;
		case TOKEN_KEYWORD_EXTERN:
			printf("TOKEN_KEYWORD_EXTERN\n");
			break;
		case TOKEN_KEYWORD_FLOAT:
			printf("TOKEN_KEYWORD_FLOAT\n");
			break;
		case TOKEN_KEYWORD_FOR:
			printf("TOKEN_KEYWORD_FOR\n");
			break;
		case TOKEN_KEYWORD_GOTO:
			printf("TOKEN_KEYWORD_GOTO\n");
			break;
		case TOKEN_KEYWORD_IF:
			printf("TOKEN_KEYWORD_IF\n");
			break;
		case TOKEN_KEYWORD_INLINE:
			printf("TOKEN_KEYWORD_INLINE\n");
			break;
		case TOKEN_KEYWORD_INT:
			printf("TOKEN_KEYWORD_INT\n");
			break;
		case TOKEN_KEYWORD_LONG:
			printf("TOKEN_KEYWORD_LONG\n");
			break;
		case TOKEN_KEYWORD_REGISTER:
			printf("TOKEN_KEYWORD_REGISTER\n");
			break;
		case TOKEN_KEYWORD_RESTRICT:
			printf("TOKEN_KEYWORD_RESTRICT\n");
			break;
		case TOKEN_KEYWORD_RETURN:
			printf("TOKEN_KEYWORD_RETURN\n");
			break;
		case TOKEN_KEYWORD_SHORT:
			printf("TOKEN_KEYWORD_SHORT\n");
			break;
		case TOKEN_KEYWORD_SIGNED:
			printf("TOKEN_KEYWORD_SIGNED\n");
			break;
		case TOKEN_KEYWORD_SIZEOF:
			printf("TOKEN_KEYWORD_SIZEOF\n");
			break;
		case TOKEN_KEYWORD_STATIC:
			printf("TOKEN_KEYWORD_STATIC\n");
			break;
		case TOKEN_KEYWORD_STRUCT:
			printf("TOKEN_KEYWORD_STRUCT\n");
			break;
		case TOKEN_KEYWORD_SWITCH:
			printf("TOKEN_KEYWORD_SWITCH\n");
			break;
		case TOKEN_KEYWORD_TYPEDEF:
			printf("TOKEN_KEYWORD_TYPEDEF\n");
			break;
		case TOKEN_KEYWORD_UNION:
			printf("TOKEN_KEYWORD_UNION\n");
			break;
		case TOKEN_KEYWORD_UNSIGNED:
			printf("TOKEN_KEYWORD_UNSIGNED\n");
			break;
		case TOKEN_KEYWORD_VOID:
			printf("TOKEN_KEYWORD_VOID\n");
			break;
		case TOKEN_KEYWORD_VOLATILE:
			printf("TOKEN_KEYWORD_VOLATILE\n");
			break;
		case TOKEN_KEYWORD_WHILE:
			printf("TOKEN_KEYWORD_WHILE\n");
			break;
		case TOKEN_KEYWORD_BOOL:
			printf("TOKEN_KEYWORD_BOOL\n");
			break;
		case TOKEN_KEYWORD_OMPLEX:
			printf("TOKEN_KEYWORD_OMPLEX\n");
			break;
		case TOKEN_KEYWORD_IMAGINARY:
			printf("TOKEN_KEYWORD_IMAGINARY\n");
			break;
		default:
			printf("!!!Unknown!!!\n");
			return -1;
	}

	return 0;
}

static const uint8 lex_table[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 4, 4, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3,
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 2,
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
	};

enum {
	LEX_INTBYTE_ULL = 1,
	LEX_INTBYTE_UL,
	LEX_INTBYTE_U,
	LEX_INTBYTE_LL,
	LEX_INTBYTE_L,
	LEX_ERR_INTBYTE,
	LEX_ERR_FLOAT_DECIMAL,
	LEX_ERR_FLOAT_DECIMAL_EXP,
	LEX_ERR_FLOAT_HEX,
	LEX_ERR_FLOAT_HEX_EXP,
};

struct lex_tok {
	union {
		const char *str;
		uint8 _u8;
		uint16 _u16;
		uint32 _u32;
		uint64 _u64;
		uint64L _u64L;
		int8 _i8;
		int16 _i16;
		int32 _i32;
		int64 _i64;
		int64L _i64L;
		float32 _f32;
		float64 _f64;
	} u;
	union {
		int32 len;
		int32 intbyte;
	} f;
	int32 row;
	int32 col;
	int32 tok;
};

struct lex_ctx {
	const char *str;
	int32 len;
	int32 row;
	int32 col;
	int32 (*call_err)();
	struct lex_tok *tmp;
};

#undef INVALID_CHARACTER
#define INVALID_CHARACTER(x) ((uint8)(x) < 0x20)

#undef LEX_LINE
#define LEX_LINE(_ctx, _x) \
	do { \
		if ((_x) == '\n') { \
			(_ctx)->row++; \
			(_ctx)->col = (_ctx)->len + 1; \
		} \
	} while (0)

#undef LEX_ROW
#define LEX_ROW(_ctx) ((_ctx)->row)
#undef LEX_COL
#define LEX_COL(_ctx, _off) \
	((_ctx)->len + 1 - (_ctx)->col - (_off))

void lex_tok_init(struct lex_tok *t, int32 tok) {
	XSYMBOL(memset)(t, 0, sizeof(struct lex_tok));
	t->tok = tok;
}

static int32 _lex_row(const char *s) {
	const char *a = s;
	while (*s != '\0' && *s != '\n')
		s++;

	return (int32)(s - a);
}

static int32 _lex_srow(const char *s) {
	const char *a = s;
	int32 st = 0;
	for (; *s != '\0'; s++) {
		char c = *s;
		switch (st) {
			case 0:
				if (c == '\\') {
					st = 1;
				} else if (c == '\n') {
					goto e;
				}
				break;
			case 1:
				st = 0;
				break;
			default:
				goto e;
		}
	}
e:

	return (int32)(s - a);
}

static int32 _lex_float_de(struct lex_ctx *ctx) {
	int32 st = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* exponent */
				if (c == '+' || c == '-') {
					st = 1;
					break;
				} else if (c >= '0' && c <= '9') {
					st = 2;
					break;
				}

				ctx->err = LEX_ERR_FLOAT_EXP;
				return -1;
			case 1:
				if (c >= '0' && c <= '9') {
					st = 2;
					break;
				}

				CTX->ERR = LEX_ERR_FLOAT_EXP;
				return -1;
			case 2:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					case 1:
						break;
					case 2:
						if (c == 'F' || c == 'f') {
							st = 3;
							break;
						}
					default:
						ctx->err = LEX_ERR_FLOAT_EXP;
						return -1;
				}
				break;
			case 3:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					default:
						break;
				}

				ctx->err = LEX_ERR_FLOAT_CHAR;
				return -1;
			default:
				return -1;
		}
	}

	return 0;
}

int32 lex_float_d(struct lex_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* decimal */
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					case 1:
						st = 1;
						break;
					case 2:
						if (c == 'E' || c == 'e') {
							st = 3;
							break;
						}
						if (c == 'F' || c == 'f') {
							st = 2;
							break;
						}
					default:
						ctx->err = LEX_ERR_FLOAT_DECIMAL;
						return -1;
				}
				break;
			case 1:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					case 1:
						break;
					case 2:
						if (c == 'E' || c == 'e') {
							st = 3;
							break;
						}
						if (c == 'F' || c == 'f') {
							st = 2;
							break;
						}
					default:
						ctx->err = LEX_ERR_FLOAT_DECIMAL;
						return -1;
				}
				break;
			case 2:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					default:
						break;
				}

				ctx->err = LEX_ERR_FLOAT_CHAR;
				return -1;
			case 3: /* exponent */
				return lex_float_de(ctx);
			default:
				return -1;
		}
	}

	return 0;
}

int32 lex_float_he(struct lex_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* exponent */
				if (c == '+' || c == '-') {
					st = 1;
					break;
				} else if (c >= '0' && c <= '9') {
					st = 2;
					break;
				}

				ctx->err = LEX_ERR_FLOAT_HEX_EXP;
				return -1;
			case 1:
				if (c >= '0' && c <= '9') {
					st = 2;
					break;
				}

				ctx->err = LEX_ERR_FLOAT_HEX_EXP;
				return -1;
			case 2:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					case 1:
						break;
					case 2:
						if (c == 'F' || c == 'f') {
							st = 3;
							break;
						}
					default:
						return -1;
				}
				break;
			case 3:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					default:
						break;
				}

				ctx->err = LEX_ERR_FLOAT_CHAR;
				return -1;
			default:
				return -1;
		}
	}

	return 0;
}

int32 lex_float_h(struct lex_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0: /* decimal */
				if (c >= '0' && c <= '9') {
					st = 1;
					break;
				} else if (c >= 'A' && c <= 'F') {
					st = 1;
					break;
				} else if (c >= 'a' && c <= 'f') {
					st = 1;
					break;
				} else if (c == 'P' || c == 'p') {
					st = 2;
					break;
				}

				ctx->err = LEX_ERR_FLOAT_HEX;
				return -1;
			case 1:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						ctx->err = LEX_ERR_FLOAT_HEX;
						return -1;
					case 1:
						break;
					case 2:
						if (c >= 'A' && c <= 'F')
							break;
						if (c >= 'a' && c <= 'f')
							break;
						if (c == 'P' || c == 'p') {
							st = 2;
							break;
						}
					default:
						ctx->err = LEX_ERR_FLOAT_HEX;
						return -1;
				}
				break;
			case 2: /* exponent */
				return lex_float_he(ctx);
			default:
				return -1;
		}
	}

	return 0;
}

static int32 _lex_intbyte(struct lex_ctx *ctx) {
	int32 st = 0, len = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0:
				st = 1;
				if (!strncmp("ULL", s, 3)
						|| !strncmp("ull", s, 3)) {
					ctx->tmp->f.intbyte = LEX_INTBYTE_ULL;
					break;
				} else if (!strncmp("UL", s, 2)
						|| !strncmp("ul", s, 2)) {
					ctx->tmp->f.intbyte = LEX_INTBYTE_UL;
					break;
				} else if (!strncmp("U", s, 1)
						|| !strncmp("u", s, 1)) {
					ctx->tmp->f.intbyte = LEX_INTBYTE_U;
					break;
				} else if (!strncmp("LL", s, 2)
						|| !strncmp("ll", s, 2)) {
					ctx->tmp->f.intbyte = LEX_INTBYTE_LL;
					break;
				} else if (!strncmp("L", s, 1)
						|| !strncmp("l", s, 1)) {
					ctx->tmp->f.intbyte = LEX_INTBYTE_L;
					break;
				}

				/* err */
				st = 2;
				break;
			case 1:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4: /* end */
						return 0;
					default:
						break;
				}

				/* err */
				if (ctx->call_err(LEX_ERR_INTBYTE_CHAR,
						ctx->str,
						len,
						ctx->arg))
					return -2;
				return -1;
			case 2: /* err */
				switch (lex_table[(uint8)c]) {
					case 1:
					case 2:
						break;
					default:
						if (ctx->call_err(LEX_ERR_INTBYTE,
								ctx->str - len,
								len,
								ctx->arg))
							return -2;
						return -1;
				}
				break;
			default:
				return -1;
		}
	}

	return 0;
}

static int32 _lex_number(struct lex_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0:
				ctx->tmp->u.str = ctx->str;
				if (c == '0') {
					st = 5;
					break;
				} else if (c >= '0' && c <= '9') {
					st = 1;
					break;
				}

				ctx->tmp->f.len = 1;
				if (ctx->call_err(LEX_ERR_NUMBER_CHAR,
						ctx->tmp, ctx->arg))
					return -2;
				return -1;
			case 1: /* decimal */
				switch (lex_table[(uint8)c]) {
					case 3:
						if (c == '.') {
							st = 3;
							break;
						}
					case 4:
						ctx->tmp->tok = TOKEN_A_DECIMAL;
						return 0;
					case 1:
						break;
					case 2:
						if (c == 'E' || c == 'e') {
							st = 4;
							break;
						}
						if (c == 'U' || c == 'u') {
							st = 2;
							break;
						}
						if (c == 'L' || c == 'l') {
							st = 2;
							break;
						}
					default:
						ctx->err = LEX_ERR_NUMBER_CHAR;
						return -1;
				}
				break;
			case 2: /* decimal U/L */
				ctx->str--;
				ctx->len--;
				if (_lex_intbyte(ctx))
					return -1;

				ctx->tmp->tok = TOKEN_A_DECIMAL;
				return ctx->len - len;
			case 3: /* floating */
				return lex_float_d(ctx);
			case 4: /* exponent */
				return lex_float_de(ctx);
			case 5: /* octal or hexadecimal */
				switch (lex_table[(uint8)c]) {
					case 3:
						if (c == '.') {
							st = 3;
							break;
						}
					case 4:
						ctx->tmp->tok = TOKEN_A_DECIMAL;
						return ctx->len - len;
					case 1:
						if (c >= '0' && c <= '7') {
							st = 6;
							break;
						}
					case 2:
						if (c == 'X' || c == 'x') {
							st = 8;
							break;
						}
					default:
						ctx->err = LEX_ERR_NUMBER_CHAR;
						return -1;
				}
				break;
			case 6: /* octal */
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						return 0;
					case 1:
						if (c >= '0' && c <= '7')
							break;
					case 2:
						if (c == 'U' || c == 'u') {
							st = 7;
							break;
						}
						if (c == 'L' || c == 'l') {
							st = 7;
							break;
						}
					default:
						printf("%d:%d: ", ctx->row,
							ctx->len - ctx->col);

						printf("!!!octal error!!!\n");
						return -1;
				}
				break;
			case 7: /* octal U/L */
				ctx->str--;
				ctx->len--;
				if (lex_float_bitlen(ctx)) {
					printf("%d:%d: ", ctx->row,
						ctx->len - ctx->col);

					printf("!!!octal U/L error!!!\n");
					return -1;
				}

				printf("%d:%d: ", ctx->row,
					ctx->len - ctx->col);

				lex_print(TOKEN_A_OCTAL);
				return 0;
			case 8: /* hexadecimal */
				if (c >= '0' && c <= '9') {
					st = 9;
					break;
				} else if (c >= 'A' && c <= 'F') {
					st = 9;
					break;
				} else if (c >= 'a' && c <= 'f') {
					st = 9;
					break;
				}

				printf("%d:%d: ", ctx->row,
					ctx->len - ctx->col);

				printf("!!!hexadecimal error!!!\n");
				return -1;
			case 9:
				switch (lex_table[(uint8)c]) {
					case 3:
						if (c == '.') {
							st = 11;
							break;
						}
						if (c == '+') {
							printf("%d:%d: ", ctx->row,
								ctx->len - ctx->col);

							printf("!!!hexadecimal suffixe"
								" error!!!\n");
							return -1;
						}
					case 4:
						printf("%d:%d: ", ctx->row,
							ctx->len - ctx->col);

						lex_print(TOKEN_A_HEX);
						return 0;
					case 1:
						break;
					case 2:
						if (c >= 'A' && c <= 'F')
							break;
						if (c >= 'a' && c <= 'f')
							break;
						if (c == 'P' || c == 'p') {
							st = 12;
							break;
						}
						if (c == 'U' || c == 'u') {
							st = 10;
							break;
						}
						if (c == 'L' || c == 'l') {
							st = 10;
							break;
						}
					default:
						printf("%d:%d: ", ctx->row,
							ctx->len - ctx->col);

						printf("!!!hexadecimal"
							" error!!!\n");
						return -1;
				}
				break;
			case 10: /* hexadecimal U/L */
				ctx->str--;
				ctx->len--;
				if (lex_float_bitlen(ctx)) {
					printf("%d:%d: ", ctx->row,
						ctx->len - ctx->col);

					printf("!!!hexadecimal U/L error!!!\n");
					return -1;
				}

				printf("%d:%d: ", ctx->row,
					ctx->len - ctx->col);

				lex_print(TOKEN_A_HEX);
				return 0;
			case 11: /* floating */
				return lex_float_h(ctx);
			case 12: /* exponent */
				return lex_float_he(ctx);
			default:
				return -1;
		}
	}

	return 0;
}

enum {
	LEX_ST_PREFIX_ADD = 1000, /* "+"  */
	LEX_ST_PREFIX_SUB,        /* "-"  */
	LEX_ST_PREFIX_MUL,        /* "*"  */
	LEX_ST_PREFIX_DIV,        /* "/"  */
	LEX_ST_PREFIX_MOD,        /* "%"  */
	LEX_ST_PREFIX_AND,        /* "&"  */
	LEX_ST_PREFIX_OR,         /* "|"  */
	LEX_ST_PREFIX_XOR,        /* "^"  */
	LEX_ST_PREFIX_OP_LT,      /* "<"  */
	LEX_ST_PREFIX_OP_LT_LT,   /* "<<" */
	LEX_ST_PREFIX_OP_GT,      /* ">"  */
	LEX_ST_PREFIX_OP_GT_GT,   /* ">>" */
	LEX_ST_PREFIX_OP_NOT,     /* "!"  */
	LEX_ST_PREFIX_ASSIGN,     /* "="  */
	LEX_ST_PREFIX_A_DOT,      /* "."  */
	LEX_ST_PREFIX_A_DOT_DOT,  /* ".." */
	LEX_ST_PREFIX_A_COLON     /* ":"  */
};

int32 lex(struct lex_ctx *ctx) {
	int32 st = 0, k = 0, len = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0:
				switch (lex_table[(uint8)c]) {
					case 1:
						k = lex_float(ctx);
						if (k < 0)
							return -1;
						ctx->tmp->u.str = ctx->str - k;
						ctx->tmp->f.len = k;
						ctx->tmp->row = LEX_ROW(ctx);
						ctx->tmp->col = LEX_COL(ctx, k);
						ctx->str--;
						ctx->len--;
						break;
					case 2:
						len = 1;
						st = 1;
						break;
					case 3:
						len = 0;
						ctx->str--;
						ctx->len--;
						st = 8;
						break;
					case 4:
						LEX_LINE(ctx, c);
						break;
					default:
						return -1;
				}
				break;
			case 1: /* name */
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, len));

						lex_print(TOKEN_NAME);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
					case 1:
					case 2:
						len++;
						break;
					default:
						return -1;
				}
				break;
			case 2: /* string */
				if (c == '\n') {
					printf("%d:%d-%d: ",
						LEX_ROW(ctx),
						LEX_COL(ctx, len), len);

					printf("!!!string error!!!\n");
					return -1;
				}

				if (c == '"') {
					printf("%d:%d+%d: ",
						LEX_ROW(ctx),
						LEX_COL(ctx, len), len);

					lex_print(TOKEN_A_STRING);
					st = 0;
					break;
				} else if (c == '\\') {
					st = 3;
				}

				len++;
				break;
			case 3: /* string escape */
				len++;
				st = 2;
				break;
			case 4: /* char */
				if (c == '\n') {
					printf("%d:%d-%d: ",
						LEX_ROW(ctx),
						LEX_COL(ctx, len), len);

					printf("!!!string error!!!\n");
					return -1;
				}

				if (INVALID_CHARACTER(c))
					return -1;

				if (c == '\'') {
					printf("%d:%d+%d: ",
						LEX_ROW(ctx),
						LEX_COL(ctx, len), len);

					lex_print(TOKEN_A_CHAR);
					st = 0;
					break;
				} else if (c == '\\') {
					st = 5;
				}

				len++;
				break;
			case 5: /* char escape */
				len++;
				st = 4;
				break;
			case 6: /* comment */
				LEX_LINE(ctx, c);
				if (c == '*')
					st = 7;
				break;
			case 7:
				LEX_LINE(ctx, c);
				if (c == '/') {
					st = 0;
				} else {
					st = 6;
				}
				break;
			case 8:
				switch (c) {
					case '"':
						st = 2;
						break;
					case '\'':
						st = 4;
						break;
					case '+':
						st = LEX_ST_PREFIX_ADD;
						break;
					case '-':
						st = LEX_ST_PREFIX_SUB;
						break;
					case '*':
						st = LEX_ST_PREFIX_MUL;
						break;
					case '/':
						st = LEX_ST_PREFIX_DIV;
						break;
					case '%':
						st = LEX_ST_PREFIX_MOD;
						break;
					case '&':
						st = LEX_ST_PREFIX_AND;
						break;
					case '|':
						st = LEX_ST_PREFIX_OR;
						break;
					case '^':
						st = LEX_ST_PREFIX_XOR;
						break;
					case '~':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_NEG);
						st = 0;
						break;
					case '<':
						st = LEX_ST_PREFIX_OP_LT;
						break;
					case '>':
						st = LEX_ST_PREFIX_OP_GT;
						break;
					case '!':
						st = LEX_ST_PREFIX_OP_NOT;
						break;
					case '=':
						st = LEX_ST_PREFIX_ASSIGN;
						break;
					case '.':
						st = LEX_ST_PREFIX_A_DOT;
						break;
					case '?':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_MARK);
						st = 0;
						break;
					case ':':
						st = LEX_ST_PREFIX_A_COLON;
						break;
					case ',':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_COMMA);
						st = 0;
						break;
					case ';':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_SEMI);
						st = 0;
						break;
					case '#':
						k = lex_srow(ctx->str);
						printf("%d:%d++%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0), k);

						lex_print(TOKEN_A_SIGN);

						k--;
						ctx->str += k;
						ctx->len += k;
						st = 0;
						break;
					case '(':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_L_BRAK);
						st = 0;
						break;
					case ')':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_R_BRAK);
						st = 0;
						break;
					case '{':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_L_SQBRAK);
						st = 0;
						break;
					case '}':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_R_SQBRAK);
						st = 0;
						break;
					case '[':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_L_CUBRAK);
						st = 0;
						break;
					case ']':
						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_R_CUBRAK);
						st = 0;
						break;
					default:
						return -1;
				}
				break;
			case LEX_ST_PREFIX_ADD: /* prefix '+' */
				switch (c) {
					case '+':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_ADD_ADD);
						st = 0;
						break;
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_ADD_ASSIGN);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_ADD);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_SUB: /* prefix '-' */
				switch (c) {
					case '-':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_SUB_SUB);
						st = 0;
						break;
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_SUB_ASSIGN);
						st = 0;
						break;
					case '>':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_A_PTR);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_SUB);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_MUL: /* prefix '*' */
				switch (c) {
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_MUL_ASSIGN);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_MUL);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_DIV: /* prefix '/' */
				switch (c) {
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_DIV_ASSIGN);
						st = 0;
						break;
					case '/':
						k = lex_row(ctx->str);
						k--;
						ctx->str += k;
						ctx->len += k;
						st = 0;
						break;
					case '*':
						st = 6;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_DIV);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_MOD: /* prefix '%' */
				switch (c) {
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_MOD_ASSIGN);
						st = 0;
						break;
					case ':':
						k = lex_srow(ctx->str);
						printf("%d:%d++%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0), k);

						lex_print(TOKEN_A_SIGN);

						k--;
						ctx->str += k;
						ctx->len += k;
						st = 0;
						break;
					case '>':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_A_R_SQBRAK);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_MOD);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_AND: /* prefix '&' */
				switch (c) {
					case '&':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_OP_AND);
						st = 0;
						break;
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_AND_ASSIGN);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_AND);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OR: /* prefix '|' */
				switch (c) {
					case '|':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_OP_OR);
						st = 0;
						break;
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_OR_ASSIGN);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_OR);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_XOR: /* prefix '^' */
				switch (c) {
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_XOR_ASSIGN);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_XOR);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_LT: /* prefix '<' */
				switch (c) {
					case '<':
						st = LEX_ST_PREFIX_OP_LT_LT;
						break;
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 1), 2);

						lex_print(TOKEN_OP_LE);
						st = 0;
						break;
					case '%':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 1), 2);

						lex_print(TOKEN_A_L_SQBRAK);
						st = 0;
						break;
					case ':':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 1), 2);

						lex_print(TOKEN_A_L_CUBRAK);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_OP_LT);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_LT_LT: /* prefix '<<' */
				switch (c) {
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 3);

						lex_print(TOKEN_L_SHIFT_ASSIGN);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 1), 2);

						lex_print(TOKEN_L_SHIFT);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_GT: /* prefix '>' */
				switch (c) {
					case '>':
						st = LEX_ST_PREFIX_OP_GT_GT;
						break;
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_OP_GE);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_OP_GT);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_GT_GT: /* prefix '>>' */
				switch (c) {
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 3);

						lex_print(TOKEN_R_SHIFT_ASSIGN);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_R_SHIFT);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_NOT: /* prefix '!' */
				switch (c) {
					case '=':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_OP_NE);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_OP_NOT);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_ASSIGN: /* prefix '=' */
				switch (c) {
					case '=':
						printf("%d:%d: ", ctx->row,
							ctx->len - ctx->col);
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);


						lex_print(TOKEN_OP_EQ);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_ASSIGN);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_A_DOT: /* prefix '.' */
				switch (c) {
					case '.':
						st = LEX_ST_PREFIX_A_DOT_DOT;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_DOT);
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_A_DOT_DOT: /* prefix '..' */
				switch (c) {
					case '.':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 3), 3);

						lex_print(TOKEN_A_ANY);
						st = 0;
						break;
					default:
						printf("%d:%d-%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						printf("!!!prefix .. error!!!\n");
						return -1;
				}
				break;
			case LEX_ST_PREFIX_A_COLON: /* prefix ':' */
				switch (c) {
					case '%':
						printf("%d:%d+%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 2), 2);

						lex_print(TOKEN_A_R_CUBRAK);
						st = 0;
						break;
					default:
						ctx->str--;
						ctx->len--;

						printf("%d:%d: ",
							LEX_ROW(ctx),
							LEX_COL(ctx, 0));

						lex_print(TOKEN_A_COLON);
						st = 0;
						break;
				}
				break;
			default:
				return -1;
		}
	}

	return 0;
}

#undef printf

int main(void) {
	clock_t start, end;
	double time;
	struct lex_ctx ctx = {
/*		"int main(int argc, char *argv[]) {\n"
		"\tprintf(\"Hello, World\\n\");\n"
		"}\n" */ NULL,
		0, 1, 0 };

	char *s = malloc(4 << 20);
	memset(s, 0, 4 << 20);
	fread(s, 1, ((4 << 20) - 1), stdin);
	ctx.str = s;

	start = clock();
	int32 ret = lex(&ctx);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\nret: %d, len: %d, row: %d, time: %.6f\n", ret, ctx.len, ctx.row, time);

	return 0;
}
