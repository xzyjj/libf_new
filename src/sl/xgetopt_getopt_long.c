/* xgetopt_getopt_long.c - get command options implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xgetopt.h>


/* @func: _getopt_long_r (static) - command-line long option parsing
* @param1: struct getopt_long_ctx *    # getopt_long context struct
* @param2: const char *                # short option string
* @param3: struct getopt_long_option * # long option struct
* @param4: const char **               # option parameter
* @return: int32                       # -1: error, > 0: option token
*/
static int32 _getopt_long_r(struct getopt_long_ctx *ctx, const char *optstr,
		struct getopt_long_option *option, const char **optarg) {
	char e = *optstr;
	for (int32 i = 0; option[i].name; option++) {
		uint32 len = XSYMBOL(strlen)(option[i].name);
		if (XSYMBOL(strncmp)(option[i].name, ctx->opt, len))
			continue;

		if (!(ctx->opt[len] == '=' || ctx->opt[len] == '\0'))
			continue;

		ctx->index++;
		ctx->stat = 0;
		if (option[i].flag == REQUIRED_ARGUMENT) {
			if (ctx->opt[len] == '=') {
				*optarg = &ctx->opt[len + 1];
			} else if (ctx->argv[ctx->index]) {
				*optarg = ctx->argv[ctx->index++];
			}
		} else if (option[i].flag == OPTIONAL_ARGUMENT) {
			if (ctx->opt[len] == '=')
				*optarg = &ctx->opt[len + 1];
		} /* no argument */

		ctx->longindex = i;
		return option[i].tok;
	}

	/* unknown option */
	ctx->index++;
	ctx->stat = 0;
	return (e == ':') ? ':' : '?';
} /* end */

/* @func: getopt_long_r - command-line long and short option parsing
* @param1: struct getopt_long *        # getopt_long context struct
* @param2: const char *                # short option string
* @param3: struct getopt_long_option * # long option struct
* @param4: const char **               # option parameter
* @return: int32                       # -1: error, > 0: option token
*/
int32 XSYMBOL(getopt_long_r)(struct getopt_long_ctx *ctx, const char *optstr,
		struct getopt_long_option *option, const char **optarg) {
	if (ctx->index >= ctx->argc)
		return -1;

	*optarg = NULL;
	ctx->optopt = '-';
	ctx->longindex = -1;
	if (!ctx->stat) {
		ctx->opt = ctx->argv[ctx->index];
		if (!ctx->opt)
			return -1;
		if (ctx->opt[0] != '-')
			return -1;
		if (ctx->opt[1] == '-') {
			if (ctx->opt[2] == '\0')
				return -1;
			ctx->opt += 2;
			return _getopt_long_r(ctx, optstr, option, optarg);
		}
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
