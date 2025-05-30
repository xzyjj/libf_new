/* ini_parser.h - ini file format parser definitions */

#ifndef LIBF_UL_INI_PARSER_H
#define LIBF_UL_INI_PARSER_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: ini */
#undef INI_SECTION_TYPE
#define INI_SECTION_TYPE 1
#undef INI_KEY_TYPE
#define INI_KEY_TYPE 2
#undef INI_VALUE_TYPE
#define INI_VALUE_TYPE 3

#undef ini_ctx
struct ini_ctx {
	const char *str;
	int32 line;
	int32 err;
	void *arg;
	/* type, string, length, arg */
	int32 (*call)(int32, const char *, int32, void *);
};

#undef INI_NEW
#define INI_NEW(name, _call, _arg) \
	struct ini_ctx name = { \
		.call = _call, \
		.arg = _arg \
	}

#undef INI_INIT
#define INI_INIT(name, _call, _arg) \
	(name)->call = _call; \
	(name)->arg = _arg

#undef INI_ERR
#define INI_ERR(name) ((name)->err)
#undef INI_LINE
#define INI_LIEN(name) ((name)->line)
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* ini_parser.c */
extern int32 FSYMBOL(ini_parser)(struct ini_ctx *ctx, const char *s);

#ifdef __cplusplus
}
#endif


#endif
