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
	TOKEN_KEYWORD_OMPLEX,   /* "_Complex"   */
	TOKEN_KEYWORD_IMAGINARY /* "_Imaginary" */
};

struct lex_ctx {
	const char *str;
	int32 len;
	int32 row;
	int32 col;
};

#undef INVALID_CHARACTER
#define INVALID_CHARACTER(x) ((uint8)(x) < 0x20)

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

int32 lex_row(const char *s) {
	const char *a = s;

	while (*s != '\0' && *s != '\n')
		s++;

	return (int32)(s - a);
}

int32 lex_srow(const char *s) {
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

int32 lex_bitlen(const char *s) {
	if (!strncmp("ull", s, 3)
			|| !strncmp("ULL", s, 3)) {
		return 1;
	} else if (!strncmp("ul", s, 2)
			|| !strncmp("UL", s, 2)) {
		return 2;
	} else if (!strncmp("u", s, 1)
			|| !strncmp("U", s, 1)) {
		return 3;
	} else if (!strncmp("ll", s, 2)
			|| !strncmp("LL", s, 2)) {
		return 4;
	} else if (!strncmp("l", s, 1)
			|| !strncmp("L", s, 1)) {
		return 5;
	}

	return 0;
}

int32 lex_float_bitlen(struct lex_ctx *ctx) {
	int32 st = 0;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0:
				switch (lex_bitlen(ctx->str)) {
					case 1:
						ctx->str += 2;
						ctx->len += 2;
						break;
					case 2:
						ctx->str += 1;
						ctx->len += 1;
						break;
					case 3:
						break;
					case 4:
						ctx->str += 1;
						ctx->len += 1;
						break;
					case 5:
						break;
					default:
						return -1;
				}
				st = 1;
				break;
			case 1:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						return 0;
					default:
						break;
				}
			default:
				return -1;
		}
	}

	return 0;
}

int32 lex_float_de(struct lex_ctx *ctx) {
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

				printf("!!!float exponent error!!!\n");
				return -1;
			case 1:
				if (c >= '0' && c <= '9') {
					st = 2;
					break;
				}

				printf("!!!float exponent error!!!\n");
				return -1;
			case 2:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						lex_print(TOKEN_A_FLOAT);
						return 0;
					case 1:
						break;
					case 2:
						if (c == 'F' || c == 'f') {
							st = 3;
							break;
						}
					default:
						printf("!!!float exponent"
							" error!!!\n");
						return -1;
				}
				break;
			case 3:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						lex_print(TOKEN_A_FLOAT);
						return 0;
					default:
						break;
				}

				printf("!!!float exponent error!!!\n");
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
					case 4:
						lex_print(TOKEN_A_FLOAT);
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
						printf("!!!float decimal"
							" error!!!\n");
						return -1;
				}
				break;
			case 1:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						lex_print(TOKEN_A_FLOAT);
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
						printf("!!!float decimal"
							" error!!!\n");
						return -1;
				}
				break;
			case 2:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						lex_print(TOKEN_A_FLOAT);
						return 0;
					default:
						break;
				}

				printf("!!!float decimal error!!!\n");
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

				printf("!!!float hex exponent error!!!\n");
				return -1;
			case 1:
				if (c >= '0' && c <= '9') {
					st = 2;
					break;
				}

				printf("!!!float hex exponent error!!!\n");
				return -1;
			case 2:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						lex_print(TOKEN_A_FLOAT_HEX);
						return 0;
					case 1:
						break;
					case 2:
						if (c == 'F' || c == 'f') {
							st = 3;
							break;
						}
					default:
						printf("!!!float hex exponent"
							" error!!!\n");
						return -1;
				}
				break;
			case 3:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						lex_print(TOKEN_A_FLOAT);
						return 0;
					default:
						break;
				}

				printf("!!!float exponent error!!!\n");
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

				printf("!!!float hex decimal error!!!\n");
				return -1;
			case 1:
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						printf("!!!float hex decimal exp"
							" error!!!\n");
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
						printf("!!!float hex decimal"
							" error!!!\n");
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

int32 lex_float(struct lex_ctx *ctx) {
	int32 st = 0, len = ctx->len;
	for (; *(ctx->str) != '\0'; ctx->str++, ctx->len++) {
		char c = *(ctx->str);
		switch (st) {
			case 0:
				if (c == '0') {
					st = 5;
					break;
				} else if (c >= '0' && c <= '9') {
					st = 1;
					break;
				}

				printf("!!!number error!!!\n");
				return -1;
			case 1: /* decimal */
				switch (lex_table[(uint8)c]) {
					case 3:
						if (c == '.') {
							st = 3;
							break;
						}
					case 4:
						lex_print(TOKEN_A_DECIMAL);
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
						printf("!!!decimal error!!!\n");
						return -1;
				}
				break;
			case 2: /* decimal U/L */
				ctx->str--;
				ctx->len--;
				if (lex_float_bitlen(ctx)) {
					printf("!!!decimal U/L error!!!\n");
					return -1;
				}

				lex_print(TOKEN_A_DECIMAL);
				return 0;
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
						lex_print(TOKEN_A_DECIMAL);
						return 0;
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
						printf("!!!octal or hexadecimal"
							" error!!!\n");
						return -1;
				}
				break;
			case 6: /* octal */
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
						lex_print(TOKEN_A_OCTAL);
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
						printf("!!!octal error!!!\n");
						return -1;
				}
				break;
			case 7: /* octal U/L */
				ctx->str--;
				ctx->len--;
				if (lex_float_bitlen(ctx)) {
					printf("!!!octal U/L error!!!\n");
					return -1;
				}

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
							printf("!!!hexadecimal suffixe"
								" error!!!\n");
							return -1;
						}
					case 4:
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
						printf("!!!hexadecimal"
							" error!!!\n");
						return -1;
				}
				break;
			case 10: /* hexadecimal U/L */
				ctx->str--;
				ctx->len--;
				if (lex_float_bitlen(ctx)) {
					printf("!!!hexadecimal U/L error!!!\n");
					return -1;
				}

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
						if (lex_float(ctx))
							return -1;
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
						if (c == '\n')
							ctx->row++;
						break;
					default:
						return -1;
				}
				break;
			case 1: /* name */
				switch (lex_table[(uint8)c]) {
					case 3:
					case 4:
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
#if 0

				if (INVALID_CHARACTER(c))
					return -1;
#endif

				if (c == '"') {
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
				if (INVALID_CHARACTER(c))
					return -1;

				if (c == '\'') {
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
				if (c == '*')
					st = 7;
				if (c == '\n')
					ctx->row++;
				break;
			case 7:
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
						lex_print(TOKEN_A_MARK);
						st = 0;
						break;
					case ':':
						st = LEX_ST_PREFIX_A_COLON;
						break;
					case ',':
						lex_print(TOKEN_A_COMMA);
						st = 0;
						break;
					case ';':
						lex_print(TOKEN_A_SEMI);
						st = 0;
						break;
					case '#':
						lex_print(TOKEN_A_SIGN);
						k = lex_srow(ctx->str);
						ctx->str += k;
						ctx->len += k;
						st = 0;
						break;
					case '(':
						lex_print(TOKEN_A_L_BRAK);
						st = 0;
						break;
					case ')':
						lex_print(TOKEN_A_R_BRAK);
						st = 0;
						break;
					case '{':
						lex_print(TOKEN_A_L_SQBRAK);
						st = 0;
						break;
					case '}':
						lex_print(TOKEN_A_R_SQBRAK);
						st = 0;
						break;
					case '[':
						lex_print(TOKEN_A_L_CUBRAK);
						st = 0;
						break;
					case ']':
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
						lex_print(TOKEN_ADD_ADD);
						st = 0;
						break;
					case '=':
						lex_print(TOKEN_ADD_ASSIGN);
						st = 0;
						break;
					default:
						lex_print(TOKEN_ADD);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_SUB: /* prefix '-' */
				switch (c) {
					case '-':
						lex_print(TOKEN_SUB_SUB);
						st = 0;
						break;
					case '=':
						lex_print(TOKEN_SUB_ASSIGN);
						st = 0;
						break;
					case '>':
						lex_print(TOKEN_A_PTR);
						st = 0;
						break;
					default:
						lex_print(TOKEN_SUB);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_MUL: /* prefix '*' */
				switch (c) {
					case '=':
						lex_print(TOKEN_MUL_ASSIGN);
						st = 0;
						break;
					default:
						lex_print(TOKEN_MUL);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_DIV: /* prefix '/' */
				switch (c) {
					case '=':
						lex_print(TOKEN_DIV_ASSIGN);
						st = 0;
						break;
					case '/':
						k = lex_row(ctx->str);
						ctx->str += k;
						ctx->len += k;
						ctx->row++;
						st = 0;
						break;
					case '*':
						st = 6;
						break;
					default:
						lex_print(TOKEN_DIV);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_MOD: /* prefix '%' */
				switch (c) {
					case '=':
						lex_print(TOKEN_MOD_ASSIGN);
						st = 0;
						break;
					case ':':
						lex_print(TOKEN_A_SIGN);
						k = lex_srow(ctx->str);
						ctx->str += k;
						ctx->len += k;
						st = 0;
						break;
					case '>':
						lex_print(TOKEN_A_R_SQBRAK);
						st = 0;
						break;
					default:
						lex_print(TOKEN_MOD);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_AND: /* prefix '&' */
				switch (c) {
					case '&':
						lex_print(TOKEN_OP_AND);
						st = 0;
						break;
					case '=':
						lex_print(TOKEN_AND_ASSIGN);
						st = 0;
						break;
					default:
						lex_print(TOKEN_AND);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OR: /* prefix '|' */
				switch (c) {
					case '|':
						lex_print(TOKEN_OP_OR);
						st = 0;
						break;
					case '=':
						lex_print(TOKEN_OR_ASSIGN);
						st = 0;
						break;
					default:
						lex_print(TOKEN_OR);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_XOR: /* prefix '^' */
				switch (c) {
					case '=':
						lex_print(TOKEN_XOR_ASSIGN);
						st = 0;
						break;
					default:
						lex_print(TOKEN_XOR);
						ctx->str--;
						ctx->len--;
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
						lex_print(TOKEN_OP_LE);
						st = 0;
						break;
					case '%':
						lex_print(TOKEN_A_L_SQBRAK);
						st = 0;
						break;
					case ':':
						lex_print(TOKEN_A_L_CUBRAK);
						st = 0;
						break;
					default:
						lex_print(TOKEN_OP_LT);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_LT_LT: /* prefix '<<' */
				switch (c) {
					case '=':
						lex_print(TOKEN_L_SHIFT_ASSIGN);
						st = 0;
						break;
					default:
						lex_print(TOKEN_L_SHIFT);
						ctx->str--;
						ctx->len--;
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
						lex_print(TOKEN_OP_GE);
						st = 0;
						break;
					default:
						lex_print(TOKEN_OP_GT);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_GT_GT: /* prefix '>>' */
				switch (c) {
					case '=':
						lex_print(TOKEN_R_SHIFT_ASSIGN);
						st = 0;
						break;
					default:
						lex_print(TOKEN_R_SHIFT);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_OP_NOT: /* prefix '!' */
				switch (c) {
					case '=':
						lex_print(TOKEN_OP_NE);
						st = 0;
						break;
					default:
						lex_print(TOKEN_OP_NOT);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_ASSIGN: /* prefix '=' */
				switch (c) {
					case '=':
						lex_print(TOKEN_OP_EQ);
						st = 0;
						break;
					default:
						lex_print(TOKEN_ASSIGN);
						ctx->str--;
						ctx->len--;
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
						lex_print(TOKEN_A_DOT);
						ctx->str--;
						ctx->len--;
						st = 0;
						break;
				}
				break;
			case LEX_ST_PREFIX_A_DOT_DOT: /* prefix '..' */
				switch (c) {
					case '.':
						lex_print(TOKEN_A_ANY);
						st = 0;
						break;
					default:
						printf("!!!prefix .. error!!!\n");
						return -1;
				}
				break;
			case LEX_ST_PREFIX_A_COLON: /* prefix ':' */
				switch (c) {
					case '%':
						lex_print(TOKEN_A_R_CUBRAK);
						st = 0;
						break;
					default:
						lex_print(TOKEN_A_COLON);
						ctx->str--;
						ctx->len--;
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
		0, 0, 0 };

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
