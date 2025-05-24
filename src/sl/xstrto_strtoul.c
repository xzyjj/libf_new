/* xstrto_strtoul.c - string and number convert implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstrto.h>


/* @func: _strtoull (static) - string convert integer
* @param1: char *  # string pointer
* @param2: char ** # end pointer
* @param3: uint64L # overflow limit
* @param4: int32   # base type
* @return: uint64L # string to unsigned long long
*/
static uint64L _strtoull(const char *s, char **e, uint64L *m, int32 b) {
	for (; *s == ' '; s++);

	int32 neg = 0; /* negative */
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

	if (b) {
		if (*s == '0') {
			s++;
			if (*s == 'B' || *s == 'b') {
				s++;
			} else if (*s == 'X' || *s == 'x') {
				s++;
			}
		}
	} else if (*s == '0') { /* b == 0 */
		s++;
		if (*s == 'B' || *s == 'b') {
			b = 2;
			s++;
		} else if (*s == 'X' || *s == 'x') {
			b = 16;
			s++;
		} else if (*s >= '0' && *s <= '7') {
			b = 8;
		} else {
			b = 10;
		}
	} else { /* default */
		b = 10;
	}

	for (; *s == '0'; s++);

	uint64L n = 0;
	for (int32 d = 0; *s != '\0'; s++) {
		if (*s >= '0' && *s <= '9') {
			d = (*s - '0');
		} else if (*s >= 'A' && *s <= 'Z') {
			d = (*s - 'A') + 10;
		} else if (*s >= 'a' && *s <= 'z') {
			d = (*s - 'a') + 10;
		} else {
			break;
		}
		if (d >= b)
			break;

		if (n > ((*m - d) / b)) {
			*m = 0;
			break;
		}
		n = (n * b) + d;
	}

	if (e) /* end position */
		*e = (char *)s;

	return neg ? -n : n;
} /* end */

/* @func: atoi - string convert integer
* @param1: char * # string pointer
* @return: int32  # string to int
*/
int32 XSYMBOL(atoi)(const char *s) {
	uint64L m = INT32_MAX;
	int64 n = _strtoull(s, NULL, &m, 10);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: atol - string convert integer
* @param1: char * # string pointer
* @return: int64  # string to long int
*/
int64 XSYMBOL(atol)(const char *s) {
	uint64L m = INT64_MAX;
	int64 n = _strtoull(s, NULL, &m, 10);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: atoll - string convert integer
* @param1: char * # string pointer
* @return: int64L # string to long long
*/
int64L XSYMBOL(atoll)(const char *s) {
	uint64L m = INT64L_MAX;
	int64L n = _strtoull(s, NULL, &m, 10);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtol - string convert integer
* @param1: char *  # string pointer
* @param2: char ** # end pointer
* @param3: int32   # base type
* @return: int64 * # string to long int
*/
int64 XSYMBOL(strtol)(const char *s, char **e, int32 b) {
	uint64L m = INT64_MAX;
	int64 n = _strtoull(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtoul - string convert integer
* @param1: char *   # string pointer
* @param2: char **  # end pointer
* @param3: int32    # base type
* @return: uint64 * # string to unsigned long int
*/
uint64 XSYMBOL(strtoul)(const char *s, char **e, int32 b) {
	uint64L m = UINT64_MAX;
	uint64 n = _strtoull(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtoll - string convert integer
* @param1: char *   # string pointer
* @param2: char **  # end pointer
* @param3: int32    # base type
* @return: int64L * # string to long long
*/
int64L XSYMBOL(strtoll)(const char *s, char **e, int32 b) {
	uint64L m = INT64L_MAX;
	int64L n = _strtoull(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */

/* @func: strtoull - string convert integer
* @param1: char *    # string pointer
* @param2: char **   # end pointer
* @param3: int32     # base type
* @return: uint64L * # string to unsigned long long
*/
uint64L XSYMBOL(strtoull)(const char *s, char **e, int32 b) {
	uint64L m = UINT64L_MAX;
	uint64L n = _strtoull(s, e, &m, b);
	if (!m) {
		/* errno */
		n = 0;
	}

	return n;
} /* end */
