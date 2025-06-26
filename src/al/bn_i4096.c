/* bn_i4096.c - big number calculation (4096bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_4096.h>


/* @func: bn_int4096_zero - big number init
* @param1: bn_int4096_t # target
* @return:
*/
void FSYMBOL(bn_int4096_zero)(bn_int4096_t t) {
	XSYMBOL(memset)(t, 0, sizeof(uint32) * (BN_4096_SIZE + 2));
} /* end */

/* @func: bn_int4096_bits - get the bit length of big number
* @param1: bn_int4096_t # number
* @return: uint32       # bits length
*/
uint32 FSYMBOL(bn_int4096_bits)(const bn_int4096_t n) {
	return FSYMBOL(bn_uint4096_bits)(n);
} /* end */

/* @func: bn_int4096_move - big number move
* @param1: bn_int4096_t # target
* @param2: bn_int4096_t # source
* @return: void
*/
void FSYMBOL(bn_int4096_move)(bn_int4096_t t, const bn_int4096_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_4096_SIZE + 2));
	t[BN_4096_SIZE] = 0;
} /* end */

/* @func: bn_int4096_moveu - big number move (signed to unsigned)
* @param1: bn_uint4096_t # target
* @param2: bn_int4096_t  # source
* @return: void
*/
void FSYMBOL(bn_int4096_moveu)(bn_uint4096_t t, const bn_int4096_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_4096_SIZE + 2));
	if (t[BN_4096_SIG]) {
		XSYMBOL(memset)(t, 0xff, sizeof(uint32) * BN_4096_SIZE);
		t[BN_4096_SIZE] = 0;
		t[BN_4096_SIG] = 0;
	}
} /* end */

/* @func: bn_int4096_movei - big number move (unsigned to signed)
* @param1: bn_int4096_t  # target
* @param2: bn_uint4096_t # source
* @return: void
*/
void FSYMBOL(bn_int4096_movei)(bn_int4096_t t, const bn_uint4096_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_4096_SIZE + 2));
	t[BN_4096_SIZE] = 0;
	t[BN_4096_SIG] = 0;
} /* end */

/* @func: bn_int4096_cmp - big number compare
* @param1: bn_int4096_t # number (a)
* @param2: bn_int4096_t # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int4096_cmp)(const bn_int4096_t a, const bn_int4096_t b) {
	if (a[BN_4096_SIG] > b[BN_4096_SIG])
		return -1;
	if (a[BN_4096_SIG] < b[BN_4096_SIG])
		return 1;

	for (int32 i = BN_4096_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return a[BN_4096_SIG] ? -1 : 1;
		if (a[i] < b[i])
			return a[BN_4096_SIG] ? 1 : -1;
	}

	return 0;
} /* end */

/* @func: bn_int4097_cmp - big number compare
* @param1: bn_int4096_t # number (a)
* @param2: bn_int4096_t # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int4097_cmp)(const bn_int4096_t a, const bn_int4096_t b) {
	if (a[BN_4096_SIG] > b[BN_4096_SIG])
		return -1;
	if (a[BN_4096_SIG] < b[BN_4096_SIG])
		return 1;

	for (int32 i = BN_4096_SIZE; i >= 0; i--) {
		if (a[i] > b[i])
			return a[BN_4096_SIG] ? -1 : 1;
		if (a[i] < b[i])
			return a[BN_4096_SIG] ? 1 : -1;
	}

	return 0;
} /* end */

/* @func: bn_int4096_cmp_1 - big number compare
* @param1: bn_int4096_t # number (a)
* @param2: uint32       # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int4096_cmp_1)(const bn_int4096_t a, uint32 b) {
	if (a[BN_4096_SIG])
		return -1;

	for (int32 i = BN_4096_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_int4096_lsh - shift big numbers left 1bit
* @param1: bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_lsh)(bn_int4096_t n) {
	FSYMBOL(bn_uint4096_lsh)(n);
} /* end */

/* @func: bn_int4097_lsh - shift big numbers left 1bit
* @param1: bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4097_lsh)(bn_int4096_t n) {
	FSYMBOL(bn_uint4097_lsh)(n);
} /* end */

/* @func: bn_int4096_rsh - shift big numbers right 1bit
* @param1: bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_rsh)(bn_int4096_t n) {
	FSYMBOL(bn_uint4096_rsh)(n);
} /* end */

/* @func: bn_int4097_rsh - shift big numbers right 1bit
* @param1: bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4097_rsh)(bn_int4096_t n) {
	FSYMBOL(bn_uint4097_rsh)(n);
} /* end */

/* @func: bn_int4096_and - big number and operation
* @param1: bn_int4096_t       # result
* @param2: const bn_int4096_t # number
* @param3: const bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_and)(bn_int4096_t r,
		const bn_int4096_t a, const bn_int4096_t b) {
	FSYMBOL(bn_uint4096_and)(r, a, b);

	r[BN_4096_SIG] = a[BN_4096_SIG] && b[BN_4096_SIG];
	if (!FSYMBOL(bn_uint4096_cmp_1)(r, 0))
		r[BN_4096_SIG] = 0;
} /* end */

/* @func: bn_int4096_or - big number or operation
* @param1: bn_int4096_t       # result
* @param2: const bn_int4096_t # number
* @param3: const bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_or)(bn_int4096_t r,
		const bn_int4096_t a, const bn_int4096_t b) {
	FSYMBOL(bn_uint4096_or)(r, a, b);

	r[BN_4096_SIG] = a[BN_4096_SIG] || b[BN_4096_SIG];
	if (!FSYMBOL(bn_uint4096_cmp_1)(r, 0))
		r[BN_4096_SIG] = 0;
} /* end */

/* @func: bn_int4096_xor - big number xor operation
* @param1: bn_int4096_t       # result
* @param2: const bn_int4096_t # number
* @param3: const bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_xor)(bn_int4096_t r,
		const bn_int4096_t a, const bn_int4096_t b) {
	FSYMBOL(bn_uint4096_xor)(r, a, b);

	r[BN_4096_SIG] = a[BN_4096_SIG] ^ b[BN_4096_SIG];
	if (!FSYMBOL(bn_uint4096_cmp_1)(r, 0))
		r[BN_4096_SIG] = 0;
} /* end */

/* @func: bn_int4096_not - big number not operation
* @param1: bn_int4096_t       # result
* @param2: const bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_not)(bn_int4096_t r,
		const bn_int4096_t n) {
	FSYMBOL(bn_uint4096_not)(r, n);

	r[BN_4096_SIG] = !n[BN_4096_SIG];
	if (!FSYMBOL(bn_uint4096_cmp_1)(r, 0))
		r[BN_4096_SIG] = 0;
} /* end */

/* @func: bn_int4096_add - big number addition
* @param1: bn_int4096_t       # sum
* @param2: const bn_int4096_t # addend
* @param3: const bn_int4096_t # addend
* @return: void
*/
void FSYMBOL(bn_int4096_add)(bn_int4096_t r,
		const bn_int4096_t a, const bn_int4096_t b) {
	if (a[BN_4096_SIG] == b[BN_4096_SIG]) {
		r[BN_4096_SIG] = a[BN_4096_SIG];
		FSYMBOL(bn_uint4096_add)(r, a, b);
	} else {
		int32 k = FSYMBOL(bn_uint4096_cmp)(a, b);
		if (k > 0) {
			r[BN_4096_SIG] = a[BN_4096_SIG];
			FSYMBOL(bn_uint4096_sub)(r, a, b);
		} else if (k < 0) {
			r[BN_4096_SIG] = b[BN_4096_SIG];
			FSYMBOL(bn_uint4096_sub)(r, b, a);
		} else {
			FSYMBOL(bn_int4096_zero)(r);
		}
	}
} /* end */

/* @func: bn_int4096_sub - big number subtraction
* @param1: bn_int4096_t       # difference
* @param2: const bn_int4096_t # minuend
* @param3: const bn_int4096_t # subtract
* @return: void
*/
void FSYMBOL(bn_int4096_sub)(bn_int4096_t r,
		const bn_int4096_t a, const bn_int4096_t b) {
	bn_int4096_t neg;
	FSYMBOL(bn_int4096_move)(neg, b);
	neg[BN_4096_SIG] = !neg[BN_4096_SIG];

	FSYMBOL(bn_int4096_add)(r, a, neg);
} /* end */

/* @func: bn_int4096_mul - big number multiplication
* @param1: bn_int4096_t       # product
* @param3: const bn_int4096_t # multiplicand
* @param2: const bn_int4096_t # multiplier
* @return:
*/
void FSYMBOL(bn_int4096_mul)(bn_int4096_t r,
		const bn_int4096_t a, const bn_int4096_t b) {
	FSYMBOL(bn_uint4096_mul)(r, a, b);
	r[BN_4096_SIG] = a[BN_4096_SIG] ^ b[BN_4096_SIG];

	if (!FSYMBOL(bn_int4096_cmp_1)(r, 0))
		r[BN_4096_SIG] = 0;
} /* end */

/* @func: bn_int4096_div - big number division
* @param1: bn_int4096_t       # quotient
* @param2: bn_int4096_t       # remainder
* @param3: const bn_int4096_t # dividend
* @param4: const bn_int4096_t # divisor
* @return: void
*/
void FSYMBOL(bn_int4096_div)(bn_int4096_t quo, bn_int4096_t rem,
		const bn_int4096_t a, const bn_int4096_t b) {
	FSYMBOL(bn_uint4096_div)(quo, rem, a, b);
	quo[BN_4096_SIG] = a[BN_4096_SIG] ^ b[BN_4096_SIG];
	rem[BN_4096_SIG] = a[BN_4096_SIG];

	if (!FSYMBOL(bn_uint4096_cmp_1)(quo, 0))
		quo[BN_4096_SIG] = 0;
	if (!FSYMBOL(bn_uint4096_cmp_1)(rem, 0))
		rem[BN_4096_SIG] = 0;
} /* end */

/* @func: bn_int4096_divmod - big number modulus (mathematical definition)
* @param1: bn_int4096_t       # quotient
* @param2: bn_int4096_t       # remainder
* @param3: const bn_int4096_t # dividend
* @param4: const bn_int4096_t # divisor
* @return: void
*/
void FSYMBOL(bn_int4096_divmod)(bn_int4096_t quo, bn_int4096_t rem,
		const bn_int4096_t a, const bn_int4096_t b) {
	FSYMBOL(bn_int4096_div)(quo, rem, a, b);

	if (rem[BN_4096_SIG]) {
		FSYMBOL(bn_int4096_add)(rem, rem, b);
		rem[BN_4096_SIG] = 0;
	}
} /* end */

/* @func: bn_int4096_strtonum - string convert big number
* @param1: bn_int4096_t # number
* @param2: char **      # end pointer
* @param3: int32        # base type
* @return: void
*/
void FSYMBOL(bn_int4096_strtonum)(bn_int4096_t t, const char *s, char **e,
		int32 b) {
	for (; *s == ' '; s++);

	int32 neg = 0;
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

	FSYMBOL(bn_uint4096_strtonum)(t, s, e, b);
	t[BN_4096_SIG] = neg;
} /* end */

/* @func: bn_int4096_numtostr - big number to string
* @param1: char *             # string buffer
* @param2: const bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_numtostr)(char *buf, const bn_int4096_t n) {
	if (n[BN_4096_SIG])
		*buf++ = '-';

	FSYMBOL(bn_uint4096_numtostr)(buf, n);
} /* end */

/* @func: bn_int4096_numtostr_hex - big number to string (hexadecimal)
* @param2: char *             # string buffer
* @param1: const bn_int4096_t # number
* @return: void
*/
void FSYMBOL(bn_int4096_numtostr_hex)(char *buf, const bn_int4096_t n) {
	if (n[BN_4096_SIG])
		*buf++ = '-';

	FSYMBOL(bn_uint4096_numtostr_hex)(buf, n);
} /* end */
