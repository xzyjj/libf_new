/* bn_i8192.c - big number calculation (8192bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_8192.h>


/* @func: bn_int8192_zero - big number init
* @param1: bn_int8192_t # target
* @return:
*/
void FSYMBOL(bn_int8192_zero)(bn_int8192_t t) {
	XSYMBOL(memset)(t, 0, sizeof(uint32) * (BN_8192_SIZE + 2));
} /* end */

/* @func: bn_int8192_bits - get the bit length of big number
* @param1: bn_int8192_t # number
* @return: uint32       # bits length
*/
uint32 FSYMBOL(bn_int8192_bits)(const bn_int8192_t n) {
	return FSYMBOL(bn_uint8192_bits)(n);
} /* end */

/* @func: bn_int8192_move - big number move
* @param1: bn_int8192_t # target
* @param2: bn_int8192_t # source
* @return: void
*/
void FSYMBOL(bn_int8192_move)(bn_int8192_t t, const bn_int8192_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_8192_SIZE + 2));
} /* end */

/* @func: bn_int8192_moveu - big number move (signed to unsigned)
* @param1: bn_uint8192_t # target
* @param2: bn_int8192_t  # source
* @return: void
*/
void FSYMBOL(bn_int8192_moveu)(bn_uint8192_t t, const bn_int8192_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_8192_SIZE + 2));
	if (t[BN_8192_SIG]) {
		XSYMBOL(memset)(t, 0xff, sizeof(uint32) * BN_8192_SIZE);
		t[BN_8192_SIZE] = 0;
		t[BN_8192_SIG] = 0;
	}
} /* end */

/* @func: bn_int8192_movei - big number move (unsigned to signed)
* @param1: bn_int8192_t  # target
* @param2: bn_uint8192_t # source
* @return: void
*/
void FSYMBOL(bn_int8192_movei)(bn_int8192_t t, const bn_uint8192_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_8192_SIZE + 2));
	t[BN_8192_SIG] = 0;
} /* end */

/* @func: bn_int8192_cmp - big number compare
* @param1: bn_int8192_t # number (a)
* @param2: bn_int8192_t # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int8192_cmp)(const bn_int8192_t a, const bn_int8192_t b) {
	if (a[BN_8192_SIG] > b[BN_8192_SIG])
		return -1;
	if (a[BN_8192_SIG] < b[BN_8192_SIG])
		return 1;

	for (int32 i = BN_8192_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return a[BN_8192_SIG] ? -1 : 1;
		if (a[i] < b[i])
			return a[BN_8192_SIG] ? 1 : -1;
	}

	return 0;
} /* end */

/* @func: bn_int8193_cmp - big number compare
* @param1: bn_int8192_t # number (a)
* @param2: bn_int8192_t # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int8193_cmp)(const bn_int8192_t a, const bn_int8192_t b) {
	if (a[BN_8192_SIG] > b[BN_8192_SIG])
		return -1;
	if (a[BN_8192_SIG] < b[BN_8192_SIG])
		return 1;

	for (int32 i = BN_8192_SIZE; i >= 0; i--) {
		if (a[i] > b[i])
			return a[BN_8192_SIG] ? -1 : 1;
		if (a[i] < b[i])
			return a[BN_8192_SIG] ? 1 : -1;
	}

	return 0;
} /* end */

/* @func: bn_int8192_cmp_1 - big number compare
* @param1: bn_int8192_t # number (a)
* @param2: uint32       # number (b)
* @return: int32        # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int8192_cmp_1)(const bn_int8192_t a, uint32 b) {
	if (a[BN_8192_SIG])
		return -1;

	for (int32 i = BN_8192_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_int8192_lsh - shift big numbers left 1bit
* @param1: bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_lsh)(bn_int8192_t n) {
	FSYMBOL(bn_uint8192_lsh)(n);
} /* end */

/* @func: bn_int8193_lsh - shift big numbers left 1bit
* @param1: bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8193_lsh)(bn_int8192_t n) {
	FSYMBOL(bn_uint8193_lsh)(n);
} /* end */

/* @func: bn_int8192_rsh - shift big numbers right 1bit
* @param1: bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_rsh)(bn_int8192_t n) {
	FSYMBOL(bn_uint8192_rsh)(n);
} /* end */

/* @func: bn_int8193_rsh - shift big numbers right 1bit
* @param1: bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8193_rsh)(bn_int8192_t n) {
	FSYMBOL(bn_uint8193_rsh)(n);
} /* end */

/* @func: bn_int8192_and - big number and operation
* @param1: bn_int8192_t       # result
* @param2: const bn_int8192_t # number
* @param3: const bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_and)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b) {
	FSYMBOL(bn_uint8192_and)(r, a, b);

	r[BN_8192_SIG] = a[BN_8192_SIG] && b[BN_8192_SIG];
	if (!FSYMBOL(bn_uint8192_cmp_1)(r, 0))
		r[BN_8192_SIG] = 0;
} /* end */

/* @func: bn_int8192_or - big number or operation
* @param1: bn_int8192_t       # result
* @param2: const bn_int8192_t # number
* @param3: const bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_or)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b) {
	FSYMBOL(bn_uint8192_or)(r, a, b);

	r[BN_8192_SIG] = a[BN_8192_SIG] || b[BN_8192_SIG];
	if (!FSYMBOL(bn_uint8192_cmp_1)(r, 0))
		r[BN_8192_SIG] = 0;
} /* end */

/* @func: bn_int8192_xor - big number xor operation
* @param1: bn_int8192_t       # result
* @param2: const bn_int8192_t # number
* @param3: const bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_xor)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b) {
	FSYMBOL(bn_uint8192_xor)(r, a, b);

	r[BN_8192_SIG] = a[BN_8192_SIG] ^ b[BN_8192_SIG];
	if (!FSYMBOL(bn_uint8192_cmp_1)(r, 0))
		r[BN_8192_SIG] = 0;
} /* end */

/* @func: bn_int8192_not - big number not operation
* @param1: bn_int8192_t       # result
* @param2: const bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_not)(bn_int8192_t r,
		const bn_int8192_t n) {
	FSYMBOL(bn_uint8192_not)(r, n);

	r[BN_8192_SIG] = !n[BN_8192_SIG];
	if (!FSYMBOL(bn_uint8192_cmp_1)(r, 0))
		r[BN_8192_SIG] = 0;
} /* end */

/* @func: bn_int8192_add - big number addition
* @param1: bn_int8192_t       # sum
* @param2: const bn_int8192_t # addend
* @param3: const bn_int8192_t # addend
* @return: void
*/
void FSYMBOL(bn_int8192_add)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b) {
	if (a[BN_8192_SIG] == b[BN_8192_SIG]) {
		r[BN_8192_SIG] = a[BN_8192_SIG];
		FSYMBOL(bn_uint8192_add)(r, a, b);
	} else {
		int32 k = FSYMBOL(bn_uint8192_cmp)(a, b);
		if (k > 0) {
			r[BN_8192_SIG] = a[BN_8192_SIG];
			FSYMBOL(bn_uint8192_sub)(r, a, b);
		} else if (k < 0) {
			r[BN_8192_SIG] = b[BN_8192_SIG];
			FSYMBOL(bn_uint8192_sub)(r, b, a);
		} else {
			FSYMBOL(bn_int8192_zero)(r);
		}
	}
} /* end */

/* @func: bn_int8192_sub - big number subtraction
* @param1: bn_int8192_t       # difference
* @param2: const bn_int8192_t # minuend
* @param3: const bn_int8192_t # subtract
* @return: void
*/
void FSYMBOL(bn_int8192_sub)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b) {
	bn_int8192_t neg;
	FSYMBOL(bn_int8192_move)(neg, b);
	neg[BN_8192_SIG] = !neg[BN_8192_SIG];

	FSYMBOL(bn_int8192_add)(r, a, neg);
} /* end */

/* @func: bn_int8192_mul - big number multiplication
* @param1: bn_int8192_t       # product
* @param3: const bn_int8192_t # multiplicand
* @param2: const bn_int8192_t # multiplier
* @return:
*/
void FSYMBOL(bn_int8192_mul)(bn_int8192_t r,
		const bn_int8192_t a, const bn_int8192_t b) {
	FSYMBOL(bn_uint8192_mul)(r, a, b);
	r[BN_8192_SIG] = a[BN_8192_SIG] ^ b[BN_8192_SIG];

	if (!FSYMBOL(bn_int8192_cmp_1)(r, 0))
		r[BN_8192_SIG] = 0;
} /* end */

/* @func: bn_int8192_div - big number division
* @param1: bn_int8192_t       # quotient
* @param2: bn_int8192_t       # remainder
* @param3: const bn_int8192_t # dividend
* @param4: const bn_int8192_t # divisor
* @return: void
*/
void FSYMBOL(bn_int8192_div)(bn_int8192_t quo, bn_int8192_t rem,
		const bn_int8192_t a, const bn_int8192_t b) {
	FSYMBOL(bn_uint8192_div)(quo, rem, a, b);
	quo[BN_8192_SIG] = a[BN_8192_SIG] ^ b[BN_8192_SIG];
	rem[BN_8192_SIG] = a[BN_8192_SIG];

	if (!FSYMBOL(bn_uint8192_cmp_1)(quo, 0))
		quo[BN_8192_SIG] = 0;
	if (!FSYMBOL(bn_uint8192_cmp_1)(rem, 0))
		rem[BN_8192_SIG] = 0;
} /* end */

/* @func: bn_int8192_divmod - big number modulus (mathematical definition)
* @param1: bn_int8192_t       # quotient
* @param2: bn_int8192_t       # remainder
* @param3: const bn_int8192_t # dividend
* @param4: const bn_int8192_t # divisor
* @return: void
*/
void FSYMBOL(bn_int8192_divmod)(bn_int8192_t quo, bn_int8192_t rem,
		const bn_int8192_t a, const bn_int8192_t b) {
	FSYMBOL(bn_int8192_div)(quo, rem, a, b);

	if (rem[BN_8192_SIG]) {
		FSYMBOL(bn_int8192_add)(rem, rem, b);
		rem[BN_8192_SIG] = 0;
	}
} /* end */

/* @func: bn_int8192_strtonum - string convert big number
* @param1: bn_int8192_t # number
* @param2: char **      # end pointer
* @param3: int32        # base type
* @return: void
*/
void FSYMBOL(bn_int8192_strtonum)(bn_int8192_t t, const char *s, char **e,
		int32 b) {
	for (; *s == ' '; s++);

	int32 neg = 0;
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

	FSYMBOL(bn_uint8192_strtonum)(t, s, e, b);
	t[BN_8192_SIG] = neg;
} /* end */

/* @func: bn_int8192_numtostr - big number to string
* @param1: char *             # string buffer
* @param2: const bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_numtostr)(char *buf, const bn_int8192_t n) {
	if (n[BN_8192_SIG])
		*buf++ = '-';

	FSYMBOL(bn_uint8192_numtostr)(buf, n);
} /* end */

/* @func: bn_int8192_numtostr_hex - big number to string (hexadecimal)
* @param2: char *             # string buffer
* @param1: const bn_int8192_t # number
* @return: void
*/
void FSYMBOL(bn_int8192_numtostr_hex)(char *buf, const bn_int8192_t n) {
	if (n[BN_8192_SIG])
		*buf++ = '-';

	FSYMBOL(bn_uint8192_numtostr_hex)(buf, n);
} /* end */
