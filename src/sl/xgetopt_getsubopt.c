/* xgetopt_getsubopt.c - get command options implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xgetopt.h>


/* @func: getsubopt - parse suboption arguments from a string
* @param1: char **       # options string
* @param2: char *const * # options token
* @param3: char **       # argument value
* @return: int32         # -1: option error, >= 0: token index
*/
int32 XSYMBOL(getsubopt)(char **optp, char *const *toks, char **valp) {
	*valp = NULL;
	char *p = *optp;
	if (*p == '\0')
		return -1;

	char *end = p;
	char *arg = NULL;
	for (; *end == ' '; end++, p++); /* skip prefix space */
	for (; *end != '\0'; end++) {
		if (*end == ',') { /* option: 'h,,h=,,,' */
			if (p == end) {
				p = end + 1;
				continue;
			}
			*end = '\0';
			end++;
			break;
		}

		if (arg && *end == ' ') /* argument: ,h'= ... ', */
			*end = '\0';
		if (*end == ' ' && !arg) /* argument: ,h' =...', */
			arg = end + 1;
		if (*end == '=' && !arg) /* argument: ,h'=...', */
			arg = end + 1;
	}

	for (int32 i = 0; toks[i]; i++) { /* compare token */
		if (!XSYMBOL(strcmp)(toks[i], p)) {
			*valp = arg;
			if (arg && arg[-1] == ' ')
				*valp = NULL;
			*optp = end;
			return i;
		}
	}
	*optp = end;

	return -1;
} /* end */
