/* internal_atoi.c - string to number conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/internal.h>


/* @func: internal_strtoull - string convert integer
* @param1: const char * # string pointer
* @param2: char **      # end pointer
* @param3: uint64L      # overflow limit
* @param4: int32        # base type
* @return: uint64L      # string to unsigned long long
*/
uint64L XSYMBOL(internal_strtoull)(const char *s, char **e, uint64L *m,
		int32 b) {
	for (; *s == ' ' || *s == '\t'; s++);

	int32 neg = 0; /* negative */
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

	if (b) {
		if (*s == '0') {
			s++;
			if (*s == 'B' || *s == 'b') {
				if (b != 2)
					goto e;
				s++;
			} else if (*s == 'X' || *s == 'x') {
				if (b != 16)
					goto e;
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

		if (n > ((*m - d) / b))
			*m = 0;
		n = (n * b) + d;
	}

e:
	if (e) /* end position */
		*e = (char *)s;

	return neg ? -n : n;
} /* end */
