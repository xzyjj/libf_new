/* internal_ftoa.c - floating to string conversion implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdlib.h>
#include <libf/sl/xmath.h>
#include <libf/sl/internal.h>


/* @func: _out_pad (static) - padding character
* @param1: int32  # size offset
* @param2: char * # buffer pointer
* @param3: char   # padding character
* @param4: int32  # padding length
* @return: int32  # string length (+offset)
*/
static int32 _out_pad(int32 n, char *p, char c, int32 x) {
	while (x-- > 0)
		p[n++] = c;

	return n;
} /* end */

/* @func: internal_fltostr_num - float64 convert string (decimal)
* @param1: int32   # size offset
* @param2: char *  # buffer pointer
* @param3: float64 # float64 value
* @param4: int32   # precision (max: FLTO_PREMAX)
* @return: int32   # string length (+offset)
*/
int32 XSYMBOL(internal_fltostr_num)(int32 n, char *p, float64 v, int32 pre) {
	uint32 big[90], *a, *z, *r;
	XSYMBOL(memset)(big, 0, sizeof(big));
	int32 e = 0, intlen = 0;

	switch (XSYMBOL(fpclassify)(v)) {
		case XFP_INFINITE:
			XSYMBOL(memcpy)(p, "inf", 3);
			return 3;
		case XFP_NAN:
			XSYMBOL(memcpy)(p, "nan", 3);
			return 3;
		default:
			break;
	}

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	pre = XSYMBOL(abs)(pre);
	pre = MIN(pre, FLTO_PREMAX);

	v = XSYMBOL(fabs)(v);
	v = XSYMBOL(frexp)(v, &e) * 2;
	if (v < 0) {
		a = z = r = big;
	} else {
		a = z = r = big + (sizeof(big) / sizeof(*big))
			- XFP_DBL_MANT_DIG - 1;
	}

	/* integer length (e * log10(2)) */
	intlen = e ? ((e * 0.3010299956639812) + ((e > 0) ? 1 : 0)) : 0;
	e--;

	/* enlarge */
	do {
		*z = (uint32)v; /* integer */
		v = (v - *z++) * 1000000000;
	} while (v);

	/* positive */
	while (e > 0) {
		uint32 carry = 0, sh = MIN(29, e);
		for (uint32 *d = z - 1; d >= a; d--) {
			uint64L k = ((uint64L)*d << sh) + carry;
			*d = k % 1000000000;
			carry = k / 1000000000;
		}
		if (carry)
			*--a = carry;
		while (z > a && !z[-1])
			z--;
		e -= sh;
	}

	/* max precision */
	uint32 need = ((pre + (XFP_DBL_MANT_DIG / 3) + 8) / 9) + 1;
	/* negative */
	while (e < 0) {
		uint32 carry = 0, sh = MIN(9, -e);
		for (uint32 *d = a; d < z; d++) {
			uint32 k = *d & ((1 << sh) - 1);
			*d = (*d >> sh) + carry;
			carry = (1000000000 >> sh) * k;
		}
		if (!*a)
			a++;
		if (carry)
			*z++ = carry;
		if ((z - r) > need)
			z = r + need;
		e += sh;
	}

	/* integer to string */
	int32 len = n;
	if (intlen <= 0) {
		p[len++] = '0';
		if (pre > 0) { /* prefix padding */
			p[len++] = '.';
			len = _out_pad(len, p, '0',
				MIN(pre, XSYMBOL(abs)(intlen)));
			pre -= MIN(pre, XSYMBOL(abs)(intlen));
		}
	}

	char buf[12];
	for (uint32 *d = a; d < z; d++) {
		/* get length */
		int32 k = XSYMBOL(internal_ulltostr_d)(0, buf, *d);

		if (d != a && k < 9) {
			XSYMBOL(internal_ulltostr_d)(9 - k, buf, *d);
			_out_pad(0, buf, '0', 9 - k);
			k = 9;
		}
		buf[k] = '\0';

		if (intlen > 0) { /* integer */
			XSYMBOL(memcpy)(p + len, buf, k);
			len += k;
			intlen -= k;
			if (intlen <= 0)
				p[len++] = '.';
		} else if (pre > 0) { /* decimals */
			XSYMBOL(memcpy)(p + len, buf, MIN(k, pre));
			len += MIN(k, pre);
			pre -= k;
		} else {
			break;
		}
	}
	if (pre > 0) /* precision padding */
		len = _out_pad(len, p, '0', pre);

	return len;
} /* end */
