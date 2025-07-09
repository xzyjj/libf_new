/* xstdlib_atoi.c - string to number conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdlib.h>
#include <libf/sl/internal.h>


/* @func: atoi - string convert integer
* @param1: const char * # string pointer
* @return: int32        # string to int
*/
int32 XSYMBOL(atoi)(const char *s) {
	uint64L m = INT32_MAX;
	int64 n = XSYMBOL(internal_strtoull)(s, NULL, &m, 10);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: atol - string convert integer
* @param1: const char * # string pointer
* @return: int64        # string to long int
*/
int64 XSYMBOL(atol)(const char *s) {
	uint64L m = INT64_MAX;
	int64 n = XSYMBOL(internal_strtoull)(s, NULL, &m, 10);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: atoll - string convert integer
* @param1: const char * # string pointer
* @return: int64L       # string to long long
*/
int64L XSYMBOL(atoll)(const char *s) {
	uint64L m = INT64L_MAX;
	int64L n = XSYMBOL(internal_strtoull)(s, NULL, &m, 10);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtol - string convert integer
* @param1: const char * # string pointer
* @param2: char **      # end pointer
* @param3: int32        # base type
* @return: int64 *      # string to long int
*/
int64 XSYMBOL(strtol)(const char *s, char **e, int32 b) {
	uint64L m = INT64_MAX;
	int64 n = XSYMBOL(internal_strtoull)(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtoul - string convert integer
* @param1: const char * # string pointer
* @param2: char **      # end pointer
* @param3: int32        # base type
* @return: uint64 *     # string to unsigned long int
*/
uint64 XSYMBOL(strtoul)(const char *s, char **e, int32 b) {
	uint64L m = UINT64_MAX;
	uint64 n = XSYMBOL(internal_strtoull)(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtoll - string convert integer
* @param1: const char * # string pointer
* @param2: char **      # end pointer
* @param3: int32        # base type
* @return: int64L *     # string to long long
*/
int64L XSYMBOL(strtoll)(const char *s, char **e, int32 b) {
	uint64L m = INT64L_MAX;
	int64L n = XSYMBOL(internal_strtoull)(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtoull - string convert integer
* @param1: const char * # string pointer
* @param2: char **      # end pointer
* @param3: int32        # base type
* @return: uint64L *    # string to unsigned long long
*/
uint64L XSYMBOL(strtoull)(const char *s, char **e, int32 b) {
	uint64L m = UINT64L_MAX;
	uint64L n = XSYMBOL(internal_strtoull)(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */
