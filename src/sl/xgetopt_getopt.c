/* xgetopt_getopt.c - get command options implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xgetopt.h>


/* @func: getopt_r - command-line short option parsing
* @param1: struct getopt_ctx * # getopt context struct
* @param2: const char *        # short option string
* @param3: const char **       # option parameter
* @return: int32               # -1: error, > 0: option token
*/
int32 XSYMBOL(getopt_r)(struct getopt_ctx *ctx, const char *optstr,
		const char **optarg) {
	if (ctx->index >= ctx->argc)
		return -1;

	*optarg = NULL;
	ctx->optopt = '-';
	if (!ctx->stat) {
		ctx->opt = ctx->argv[ctx->index];
		if (!ctx->opt)
			return -1;
		if (ctx->opt[0] != '-')
			return -1;
		if (ctx->opt[1] == '-') /* option: '--' */
			return -1;
		ctx->opt++;
		ctx->stat = 1;
	}
	if (ctx->opt[0] == '\0')
		return -1;

	char e = *optstr;
	for (; *optstr != '\0'; optstr++) {
		if (ctx->opt[0] != *optstr) {
			/* optstr: ':' or '::' */
			if (optstr[1] == ':') {
				if (optstr[2] == ':')
					optstr++;
				optstr++;
			}
			continue;
		}
		ctx->optopt = ctx->opt[0];

		/* next */
		if (optstr[1] == ':' || ctx->opt[1] == '\0') {
			ctx->index++;
			ctx->stat = 0;
		}
		if (optstr[1] == ':') { /* argument */
			/* optional argument: '-h'[...] */
			if (optstr[2] == ':') {
				if (ctx->opt[1] != '\0')
					*optarg = &ctx->opt[1];
			} else /* required argument: '-h'[...] */
			if (ctx->opt[1] != '\0') {
				*optarg = &ctx->opt[1];
			} else /* required argument: '-h' '...' */
			if (ctx->argv[ctx->index]) {
				*optarg = ctx->argv[ctx->index++];
			} else { /* no a argument */
				return (e == ':') ? ':' : '?';
			}
		} /* no argument */
		ctx->opt++;

		return ctx->optopt; /* option token */
	}

	/* unknown option */
	ctx->optopt = ctx->opt[0];
	if (ctx->opt[1] == '\0') {
		ctx->index++;
		ctx->stat = 0;
	}
	ctx->opt++;

	return (e == ':') ? ':' : '?';
} /* end */
