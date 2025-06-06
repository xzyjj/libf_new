/* xstrto_atof.c - string to floating conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdlib.h>
#include <libf/sl/xmath.h>


/* @func: _strtod_num (static) - string convert floating point (integer)
* @param1: char *  # string pointer
* @param2: char ** # end pointer
* @param3: int32   # base type
* @return: float64 # string to double
*/
static float64 _strtod_num(const char *s, char **e, int32 b) {
	float64 x = 0.0;
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

		x = (x * b) + d;
	}

	if (e)
		*e = (char *)s;

	return x;
} /* end */

/* @func: _strtod (static) - string convert floating point
* @param1: char *  # string pointer
* @param2: char ** # end pointer
* @return: float64 # string to double
*/
static float64 _strtod(const char *s, char **e) {
	for (; *s == ' '; s++);

	int32 neg = 0, b = 10; /* negative */
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

	if (*s == '0') {
		s++;
		if (*s == 'X' || *s == 'x') {
			b = 16;
			s++;
		}
	}

	char *ee = NULL;
	if (!e)
		e = &ee;

	/* integer and decimals */
	float64 x = _strtod_num(s, e, b);
	s = *e;
	if (*s == '.') {
		s++;
		x += _strtod_num(s, e, b) / XSYMBOL(pow)(b, *e - s);
	}

	/* exponent */
	s = *e;
	if (b == 10 && (*s == 'E' || *s == 'e')) {
		s++;
		int32 n = XSYMBOL(strtol)(s, e, 10);
		x *= XSYMBOL(pow)(10, n);
	} else if (b == 16 && (*s == 'P' || *s == 'p')) {
		s++;
		int32 n = XSYMBOL(strtol)(s, e, 10);
		x *= XSYMBOL(pow)(2, n);
	}

	return neg ? -x : x;
} /* end */

/* @func: atof - string convert floating point
* @param1: char *  # string pointer
* @return: float64 # string to double
*/
float64 XSYMBOL(atof)(const char *s) {
	return _strtod(s, NULL);
} /* end */

/* @func: strtof - string convert floating point
* @param1: char *  # string pointer
* @param2: char ** # end pointer
* @return: float32 # string to float
*/
float32 XSYMBOL(strtof)(const char *s, char **e) {
	return _strtod(s, e);
} /* end */

/* @func: strtod - string convert floating point
* @param1: char *  # string pointer
* @param2: char ** # end pointer
* @return: float64 # string to double
*/
float64 XSYMBOL(strtod)(const char *s, char **e) {
	return _strtod(s, e);
} /* end */
