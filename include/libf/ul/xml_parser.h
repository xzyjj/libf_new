/* xml_parser.h - xml (extensible markup language) parser implements */

#ifndef LIBF_UL_XML_PARSER_H
#define LIBF_UL_XML_PARSER_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: xml */
#undef XML_ELEMENT_START
#define XML_ELEMENT_START 1
#undef XML_ELEMENT_END
#define XML_ELEMENT_END 2
#undef XML_ELEMENT_EMPTY_END
#define XML_ELEMENT_EMPTY_END 3

#undef XML_ELEMENT_ATTR_NAME
#define XML_ELEMENT_ATTR_NAME 4
#undef XML_ELEMENT_ATTR_VALUE
#define XML_ELEMENT_ATTR_VALUE 5

#undef XML_ELEMENT_STRING
#define XML_ELEMENT_STRING 6

#undef XML_DOCTYPE_START
#define XML_DOCTYPE_START 7
#undef XML_DOCTYPE_NAME
#define XML_DOCTYPE_NAME 8
#undef XML_DOCTYPE_STRING
#define XML_DOCTYPE_STRING 9

#undef XML_STATEMENT_START
#define XML_STATEMENT_START 10
#undef XML_STATEMENT_ATTR_NAME
#define XML_STATEMENT_ATTR_NAME 11
#undef XML_STATEMENT_ATTR_VALUE
#define XML_STATEMENT_ATTR_VALUE 12

#undef XML_COMMENT
#define XML_COMMENT 13
#undef XML_CDATA
#define XML_CDATA 14

/* xml error code */
#undef XML_ERR_TOKEN
#define XML_ERR_TOKEN 1

#undef XML_ERR_ELEMENT_START
#define XML_ERR_ELEMENT_START 2
#undef XML_ERR_ELEMENT_CHAR
#define XML_ERR_ELEMENT_CHAR 3
#undef XML_ERR_ELEMENT_EMPTY_END
#define XML_ERR_ELEMENT_EMPTY_END 4

#undef XML_ERR_ELEMENT_ATTR_CHAR
#define XML_ERR_ELEMENT_ATTR_CHAR 5
#undef XML_ERR_ELEMENT_ATTR_VALUE
#define XML_ERR_ELEMENT_ATTR_VALUE 6

#undef XML_ERR_END_ELEMENT_START
#define XML_ERR_END_ELEMENT_START 7
#undef XML_ERR_END_ELEMENT_CHAR
#define XML_ERR_END_ELEMENT_CHAR 8

#undef XML_ERR_DOCTYPE_START
#define XML_ERR_DOCTYPE_START 9
#undef XML_ERR_DOCTYPE_CHAR
#define XML_ERR_DOCTYPE_CHAR 10
#undef XML_ERR_DOCTYPE_STRING
#define XML_ERR_DOCTYPE_STRING 11

#undef XML_ERR_STATEMENT_START
#define XML_ERR_STATEMENT_START 12
#undef XML_ERR_STATEMENT_CHAR
#define XML_ERR_STATEMENT_CHAR 13

#undef XML_ERR_STATEMENT_ATTR_CHAR
#define XML_ERR_STATEMENT_ATTR_CHAR 14
#undef XML_ERR_STATEMENT_ATTR_VALUE
#define XML_ERR_STATEMENT_ATTR_VALUE 15

#undef XML_ERR_COMMENT
#define XML_ERR_COMMENT 16
#undef XML_ERR_CDATA
#define XML_ERR_CDATA 17

struct xml_ctx {
	const char *str;
	int32 len;
	int32 err;
	void *arg;
	/* type, string, length, arg */
	int32 (*call)(int32, const char *, int32, void *);
	/* type, string, length, arg */
	int32 (*call_end)(int32, const char *, int32, void *);
};

#undef XML_NEW
#define XML_NEW(name, _call, _call_end, _arg) \
	struct xml_ctx name = { \
		.call = _call, \
		.call_end = _call_end, \
		.arg = _arg \
	}

#undef XML_INIT
#define XML_INIT(name, _call, _call_end, _arg) \
	(name)->call = _call; \
	(name)->call_end = _call_end; \
	(name)->arg = _arg

#undef XML_ERR
#define XML_ERR(name) ((name)->err)
#undef XML_LEN
#define XML_LEN(name) ((name)->len)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xml_parser.c */
extern int32 FSYMBOL(xml_parser)(struct xml_ctx *ctx, const char *s);

#ifdef __cplusplus
}
#endif


#endif
