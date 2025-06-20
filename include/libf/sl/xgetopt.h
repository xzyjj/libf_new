/* xgetopt.h - get command options definitions */

#ifndef LIBF_SL_XGETOPT_H
#define LIBF_SL_XGETOPT_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: getopt */
#undef getopt_ctx
struct getopt_ctx {
	int32 argc;
	char *const *argv;
	const char *opt;
	int32 index;
	int32 stat;
	int32 optopt;
};

#undef GETOPT_NEW
#define GETOPT_NEW(name, _argc, _argv, _index) \
	struct getopt_ctx name = { \
		.argc = _argc, .argv = _argv, \
		.index = _index, .stat = 0, \
		.optopt = 0 \
	}

#undef GETOPT_INIT
#define GETOPT_INIT(x, _argc, _argv, _index) \
	(x)->argc = _argc; (x)->argv = _argv; \
	(x)->index = _index; (x)->stat = 0; \
	(x)->optopt = 0

#undef GETOPT_OPTOPT
#define GETOPT_OPTOPT(x) ((x)->optopt)
#undef GETOPT_END
#define GETOPT_END(x) (!((x)->index < (x)->argc))
#undef GETOPT_SKIP
#define GETOPT_SKIP(x) ((x)->index++)
#undef GETOPT_ERROPT
#define GETOPT_ERROPT(x) ((x)->argv[(x)->index])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xgetopt_getopt.c */
extern int32 XSYMBOL(getopt_r)(struct getopt_ctx *ctx, const char *optstr,
		const char **optarg);

#ifdef __cplusplus
}
#endif


#endif
