/* bn_i1024.c - big number calculation (1024bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_1024.h>


/* @func: bn_int1024_zero - big number init
* @param1: bn_int1024_t # target
* @return:
*/
void FSYMBOL(bn_int1024_zero)(bn_int1024_t t) {
	XSYMBOL(memset)(t, 0, sizeof(uint32) * (BN_1024_SIZE + 2));
} /* end */

/* @func: bn_int1024_bits - get the bit length of big number
* @param1: bn_int1024_t # number
* @return: uint32       # bits length
*/
uint32 FSYMBOL(bn_int1024_bits)(const bn_int1024_t n) {
	return FSYMBOL(bn_uint1024_bits)(n);
} /* end */

/* @func: bn_int1024_move - big number move
* @param1: bn_int1024_t # target
* @param2: bn_int1024_t # source
* @return: void
*/
void FSYMBOL(bn_int1024_move)(bn_int1024_t t, const bn_int1024_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_1024_SIZE + 2));
	t[BN_1024_SIZE] = 0;
} /* end */

/* @func: bn_int1024_moveu - big number move (signed to unsigned)
* @param1: bn_uint1024_t # target
* @param2: bn_int1024_t  # source
* @return: void
*/
void FSYMBOL(bn_int1024_moveu)(bn_uint1024_t t, const bn_int1024_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_1024_SIZE + 2));
	if (t[BN_1024_SIG]) {
		XSYMBOL(memset)(t, 0xff, sizeof(uint32) * BN_1024_SIZE);
		t[BN_1024_SIZE] = 0;
		t[BN_1024_SIG] = 0;
	}
} /* end */

/* @func: bn_int1024_movei - big number move (unsigned to signed)
* @param1: bn_int1024_t  # target
* @param2: bn_uint1024_t # source
* @return: void
*/
void FSYMBOL(bn_int1024_movei)(bn_int1024_t t, const bn_uint1024_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_1024_SIZE + 2));
	t[BN_1024_SIZE] = 0;
	t[BN_1024_SIG] = 0;
} /* end */

/* @func: bn_int1024_cmp - big number compare
* @param1: bn_int1024_t # number (a)
* @param2: bn_int1024_t # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int1024_cmp)(const bn_int1024_t a, const bn_int1024_t b) {
	if (a[BN_1024_SIG] > b[BN_1024_SIG])
		return -1;
	if (a[BN_1024_SIG] < b[BN_1024_SIG])
		return 1;

	for (int32 i = BN_1024_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return a[BN_1024_SIG] ? -1 : 1;
		if (a[i] < b[i])
			return a[BN_1024_SIG] ? 1 : -1;
	}

	return 0;
} /* end */

/* @func: bn_int1025_cmp - big number compare
* @param1: bn_int1024_t # number (a)
* @param2: bn_int1024_t # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int1025_cmp)(const bn_int1024_t a, const bn_int1024_t b) {
	if (a[BN_1024_SIG] > b[BN_1024_SIG])
		return -1;
	if (a[BN_1024_SIG] < b[BN_1024_SIG])
		return 1;

	for (int32 i = BN_1024_SIZE; i >= 0; i--) {
		if (a[i] > b[i])
			return a[BN_1024_SIG] ? -1 : 1;
		if (a[i] < b[i])
			return a[BN_1024_SIG] ? 1 : -1;
	}

	return 0;
} /* end */

/* @func: bn_int1024_cmp_1 - big number compare
* @param1: bn_int1024_t # number (a)
* @param2: uint32       # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int1024_cmp_1)(const bn_int1024_t a, uint32 b) {
	if (a[BN_1024_SIG])
		return -1;

	for (int32 i = BN_1024_SIZE - 1; i >= 0; i--) {
		if (i) {
			if (a[i])
				return 1;
		} else {
			if (a[i] > b)
				return 1;
			if (a[i] < b)
				return -1;
		}
	}

	return 0;
} /* end */

/* @func: bn_int1024_lsh - shift big numbers left 1bit
* @param1: bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_lsh)(bn_int1024_t n) {
	FSYMBOL(bn_uint1024_lsh)(n);
} /* end */

/* @func: bn_int1025_lsh - shift big numbers left 1bit
* @param1: bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1025_lsh)(bn_int1024_t n) {
	FSYMBOL(bn_uint1025_lsh)(n);
} /* end */

/* @func: bn_int1024_rsh - shift big numbers right 1bit
* @param1: bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_rsh)(bn_int1024_t n) {
	FSYMBOL(bn_uint1024_rsh)(n);
} /* end */

/* @func: bn_int1025_rsh - shift big numbers right 1bit
* @param1: bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1025_rsh)(bn_int1024_t n) {
	FSYMBOL(bn_uint1025_rsh)(n);
} /* end */

/* @func: bn_int1024_and - big number and operation
* @param1: bn_int1024_t       # result
* @param2: const bn_int1024_t # number
* @param3: const bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_and)(bn_int1024_t r,
		const bn_int1024_t a, const bn_int1024_t b) {
	FSYMBOL(bn_uint1024_and)(r, a, b);

	r[BN_1024_SIG] = a[BN_1024_SIG] && b[BN_1024_SIG];
	if (!FSYMBOL(bn_uint1024_cmp_1)(r, 0))
		r[BN_1024_SIG] = 0;
} /* end */

/* @func: bn_int1024_or - big number or operation
* @param1: bn_int1024_t       # result
* @param2: const bn_int1024_t # number
* @param3: const bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_or)(bn_int1024_t r,
		const bn_int1024_t a, const bn_int1024_t b) {
	FSYMBOL(bn_uint1024_or)(r, a, b);

	r[BN_1024_SIG] = a[BN_1024_SIG] || b[BN_1024_SIG];
	if (!FSYMBOL(bn_uint1024_cmp_1)(r, 0))
		r[BN_1024_SIG] = 0;
} /* end */

/* @func: bn_int1024_xor - big number xor operation
* @param1: bn_int1024_t       # result
* @param2: const bn_int1024_t # number
* @param3: const bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_xor)(bn_int1024_t r,
		const bn_int1024_t a, const bn_int1024_t b) {
	FSYMBOL(bn_uint1024_xor)(r, a, b);

	r[BN_1024_SIG] = a[BN_1024_SIG] ^ b[BN_1024_SIG];
	if (!FSYMBOL(bn_uint1024_cmp_1)(r, 0))
		r[BN_1024_SIG] = 0;
} /* end */

/* @func: bn_int1024_not - big number not operation
* @param1: bn_int1024_t       # result
* @param2: const bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_not)(bn_int1024_t r,
		const bn_int1024_t n) {
	FSYMBOL(bn_uint1024_not)(r, n);

	r[BN_1024_SIG] = !n[BN_1024_SIG];
	if (!FSYMBOL(bn_uint1024_cmp_1)(r, 0))
		r[BN_1024_SIG] = 0;
} /* end */

/* @func: bn_int1024_add - big number addition
* @param1: bn_int1024_t       # sum
* @param2: const bn_int1024_t # addend
* @param3: const bn_int1024_t # addend
* @return: void
*/
void FSYMBOL(bn_int1024_add)(bn_int1024_t r,
		const bn_int1024_t a, const bn_int1024_t b) {
	if (a[BN_1024_SIG] == b[BN_1024_SIG]) {
		r[BN_1024_SIG] = a[BN_1024_SIG];
		FSYMBOL(bn_uint1024_add)(r, a, b);
	} else {
		int32 k = FSYMBOL(bn_uint1024_cmp)(a, b);
		if (k > 0) {
			r[BN_1024_SIG] = a[BN_1024_SIG];
			FSYMBOL(bn_uint1024_sub)(r, a, b);
		} else if (k < 0) {
			r[BN_1024_SIG] = b[BN_1024_SIG];
			FSYMBOL(bn_uint1024_sub)(r, b, a);
		} else {
			FSYMBOL(bn_int1024_zero)(r);
		}
	}
} /* end */

/* @func: bn_int1024_sub - big number subtraction
* @param1: bn_int1024_t       # difference
* @param2: const bn_int1024_t # minuend
* @param3: const bn_int1024_t # subtract
* @return: void
*/
void FSYMBOL(bn_int1024_sub)(bn_int1024_t r,
		const bn_int1024_t a, const bn_int1024_t b) {
	bn_int1024_t neg;
	FSYMBOL(bn_int1024_move)(neg, b);
	neg[BN_1024_SIG] = !neg[BN_1024_SIG];

	FSYMBOL(bn_int1024_add)(r, a, neg);
} /* end */

/* @func: bn_int1024_mul - big number multiplication
* @param1: bn_int1024_t       # product
* @param3: const bn_int1024_t # multiplicand
* @param2: const bn_int1024_t # multiplier
* @return:
*/
void FSYMBOL(bn_int1024_mul)(bn_int1024_t r,
		const bn_int1024_t a, const bn_int1024_t b) {
	FSYMBOL(bn_uint1024_mul)(r, a, b);
	r[BN_1024_SIG] = a[BN_1024_SIG] ^ b[BN_1024_SIG];

	if (!FSYMBOL(bn_int1024_cmp_1)(r, 0))
		r[BN_1024_SIG] = 0;
} /* end */

/* @func: bn_int1024_div - big number division
* @param1: bn_int1024_t       # quotient
* @param2: bn_int1024_t       # remainder
* @param3: const bn_int1024_t # dividend
* @param4: const bn_int1024_t # divisor
* @return: void
*/
void FSYMBOL(bn_int1024_div)(bn_int1024_t quo, bn_int1024_t rem,
		const bn_int1024_t a, const bn_int1024_t b) {
	FSYMBOL(bn_uint1024_div)(quo, rem, a, b);
	quo[BN_1024_SIG] = a[BN_1024_SIG] ^ b[BN_1024_SIG];
	rem[BN_1024_SIG] = a[BN_1024_SIG];

	if (!FSYMBOL(bn_uint1024_cmp_1)(quo, 0))
		quo[BN_1024_SIG] = 0;
	if (!FSYMBOL(bn_uint1024_cmp_1)(rem, 0))
		rem[BN_1024_SIG] = 0;
} /* end */

/* @func: bn_int1024_divmod - big number modulus (mathematical definition)
* @param1: bn_int1024_t       # quotient
* @param2: bn_int1024_t       # remainder
* @param3: const bn_int1024_t # dividend
* @param4: const bn_int1024_t # divisor
* @return: void
*/
void FSYMBOL(bn_int1024_divmod)(bn_int1024_t quo, bn_int1024_t rem,
		const bn_int1024_t a, const bn_int1024_t b) {
	FSYMBOL(bn_int1024_div)(quo, rem, a, b);

	if (rem[BN_1024_SIG]) {
		FSYMBOL(bn_int1024_add)(rem, rem, b);
		rem[BN_1024_SIG] = 0;
	}
} /* end */

/* @func: bn_int1024_strtonum - string convert big number
* @param1: bn_int1024_t # number
* @param2: char **      # end pointer
* @param3: int32        # base type
* @return: void
*/
void FSYMBOL(bn_int1024_strtonum)(bn_int1024_t t, const char *s, char **e,
		int32 b) {
	for (; *s == ' '; s++);

	int32 neg = 0;
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

	FSYMBOL(bn_uint1024_strtonum)(t, s, e, b);
	t[BN_1024_SIG] = neg;
} /* end */

/* @func: bn_int1024_numtostr - big number to string
* @param1: char *             # string buffer
* @param2: const bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_numtostr)(char *buf, const bn_int1024_t n) {
	if (n[BN_1024_SIG])
		*buf++ = '-';

	FSYMBOL(bn_uint1024_numtostr)(buf, n);
} /* end */

/* @func: bn_int1024_numtostr_hex - big number to string (hexadecimal)
* @param2: char *             # string buffer
* @param1: const bn_int1024_t # number
* @return: void
*/
void FSYMBOL(bn_int1024_numtostr_hex)(char *buf, const bn_int1024_t n) {
	if (n[BN_1024_SIG])
		*buf++ = '-';

	FSYMBOL(bn_uint1024_numtostr_hex)(buf, n);
} /* end */
