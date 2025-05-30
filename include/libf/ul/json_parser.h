/* json.h - json (javascript object notation) parser definitions */

#ifndef LIBF_UL_JSON_H
#define LIBF_UL_JSON_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/list.h>


/* @def: json */
#undef JSON_ARRAY_TYPE
#define JSON_ARRAY_TYPE 1
#undef JSON_OBJECT_TYPE
#define JSON_OBJECT_TYPE 2
#undef JSON_VALUE_TYPE
#define JSON_VALUE_TYPE 3
#undef JSON_STRING_TYPE
#define JSON_STRING_TYPE 4
#undef JSON_NUMBER_TYPE
#define JSON_NUMBER_TYPE 5
#undef JSON_NULL_TYPE
#define JSON_NULL_TYPE 6
#undef JSON_TRUE_TYPE
#define JSON_TRUE_TYPE 7
#undef JSON_FALSE_TYPE
#define JSON_FALSE_TYPE 8

/* json error code */
#undef JSON_ERR_START_TOKEN
#define JSON_ERR_START_TOKEN 1

#undef JSON_ERR_OBJECT_TOKEN
#define JSON_ERR_OBJECT_TOKEN 2
#undef JSON_ERR_OBJECT_START
#define JSON_ERR_OBJECT_START 3
#undef JSON_ERR_OBJECT_VALUE
#define JSON_ERR_OBJECT_VALUE 4
#undef JSON_ERR_OBJECT_END
#define JSON_ERR_OBJECT_END 5
#undef JSON_ERR_OBJECT_STRING
#define JSON_ERR_OBJECT_STRING 6
#undef JSON_ERR_OBJECT_NUMBER
#define JSON_ERR_OBJECT_NUMBER 7
#undef JSON_ERR_OBJECT_NULL
#define JSON_ERR_OBJECT_NULL 8
#undef JSON_ERR_OBJECT_TRUE
#define JSON_ERR_OBJECT_TRUE 9
#undef JSON_ERR_OBJECT_FALSE
#define JSON_ERR_OBJECT_FALSE 10

#undef JSON_ERR_ARRAY_TOKEN
#define JSON_ERR_ARRAY_TOKEN 11
#undef JSON_ERR_ARRAY_START
#define JSON_ERR_ARRAY_START 12
#undef JSON_ERR_ARRAY_END
#define JSON_ERR_ARRAY_END 13
#undef JSON_ERR_ARRAY_STRING
#define JSON_ERR_ARRAY_STRING 14
#undef JSON_ERR_ARRAY_NUMBER
#define JSON_ERR_ARRAY_NUMBER 15
#undef JSON_ERR_ARRAY_NULL
#define JSON_ERR_ARRAY_NULL 16
#undef JSON_ERR_ARRAY_TRUE
#define JSON_ERR_ARRAY_TRUE 17
#undef JSON_ERR_ARRAY_FALSE
#define JSON_ERR_ARRAY_FALSE 18

#undef JSON_ERR_INVALID
#define JSON_ERR_INVALID 19

#undef json_ctx
struct json_ctx {
	const char *str;
	int32 len;
	int32 err;
	void *arg;
	/* type, string, length, arg */
	int32 (*call)(int32, const char *, int32, void *);
	/* type, arg */
	int32 (*call_end)(int32, void *);
};

#undef JSON_NEW
#define JSON_NEW(name, _call, _call_end, _arg) \
	struct json_ctx name = { \
		.call = _call, \
		.call_end = _call_end, \
		.arg = _arg \
	}

#undef JSON_INIT
#define JSON_INIT(name, _call, _call_end, _arg) \
	(name)->call = _call; \
	(name)->call_end = _call_end; \
	(name)->arg = _arg

#undef JSON_ERR
#define JSON_ERR(name) ((name)->err)
#undef JSON_LEN
#define JSON_LEN(name) ((name)->len)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* json_parser.c */
extern int32 FSYMBOL(json_parser)(struct json_ctx *ctx, const char *str);

#ifdef __cplusplus
}
#endif


#endif
