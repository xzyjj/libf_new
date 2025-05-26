/* bn_i512.c - big number calculation (512bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_512.h>


/* @func: bn_int512_zero - big number init
* @param1: bn_int512_t # target
* @return:
*/
void FSYMBOL(bn_int512_zero)(bn_int512_t t) {
	XSYMBOL(memset)(t, 0, sizeof(uint32) * (BN_512_SIZE + 2));
} /* end */

/* @func: bn_int512_move - big number move
* @param1: bn_int512_t # target
* @param2: bn_int512_t # source
* @return: void
*/
void FSYMBOL(bn_int512_move)(bn_int512_t t, const bn_int512_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_512_SIZE + 2));
} /* end */

/* @func: bn_int512_moveu - big number move (signed to unsigned)
* @param1: bn_uint512_t # target
* @param2: bn_int512_t  # source
* @return: void
*/
void FSYMBOL(bn_int512_moveu)(bn_uint512_t t, const bn_int512_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_512_SIZE + 2));
	if (t[BN_512_SIG]) {
		XSYMBOL(memset)(t, 0xff, sizeof(uint32) * BN_512_SIZE);
		t[BN_512_SIZE] = 0;
		t[BN_512_SIG] = 0;
	}
} /* end */

/* @func: bn_int512_movei - big number move (unsigned to signed)
* @param1: bn_int512_t  # target
* @param2: bn_uint512_t # source
* @return: void
*/
void FSYMBOL(bn_int512_movei)(bn_int512_t t, const bn_uint512_t s) {
	XSYMBOL(memcpy)(t, s, sizeof(uint32) * (BN_512_SIZE + 2));
	t[BN_512_SIG] = 0;
} /* end */

/* @func: bn_int512_cmp - big number compare
* @param1: bn_int512_t # number (a)
* @param2: bn_int512_t # number (b)
* @return: int32       # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int512_cmp)(const bn_int512_t a, const bn_int512_t b) {
	if (a[BN_512_SIG] > b[BN_512_SIG])
		return -1;
	if (a[BN_512_SIG] < b[BN_512_SIG])
		return 1;

	for (int32 i = BN_512_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_int513_cmp - big number compare
* @param1: bn_int512_t # number (a)
* @param2: bn_int512_t # number (b)
* @return: int32       # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int513_cmp)(const bn_int512_t a, const bn_int512_t b) {
	if (a[BN_512_SIG] > b[BN_512_SIG])
		return -1;
	if (a[BN_512_SIG] < b[BN_512_SIG])
		return 1;

	for (int32 i = BN_512_SIZE; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_int512_cmp_1 - big number compare
* @param1: bn_int512_t # number (a)
* @param2: uint32      # number (b)
* @return: int32       # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_int512_cmp_1)(const bn_int512_t a, uint32 b) {
	if (a[BN_512_SIG])
		return -1;

	for (int32 i = BN_512_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_int512_lsh - shift big numbers left 1bit
* @param1: bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int512_lsh)(bn_int512_t n) {
	FSYMBOL(bn_uint512_lsh)(n);
} /* end */

/* @func: bn_int513_lsh - shift big numbers left 1bit
* @param1: bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int513_lsh)(bn_int512_t n) {
	FSYMBOL(bn_uint513_lsh)(n);
} /* end */

/* @func: bn_int512_rsh - shift big numbers right 1bit
* @param1: bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int512_rsh)(bn_int512_t n) {
	FSYMBOL(bn_uint512_rsh)(n);
} /* end */

/* @func: bn_int513_rsh - shift big numbers right 1bit
* @param1: bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int513_rsh)(bn_int512_t n) {
	FSYMBOL(bn_uint513_rsh)(n);
} /* end */

/* @func: bn_int512_and - big number and operation
* @param1: bn_int512_t       # result
* @param2: const bn_int512_t # number
* @param3: const bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int512_and)(bn_int512_t r, bn_int512_t a, bn_int512_t b) {
	FSYMBOL(bn_uint512_and)(r, a, b);

	r[BN_512_SIG] = a[BN_512_SIG] && b[BN_512_SIG];
	if (!FSYMBOL(bn_uint512_cmp_1)(r, 0))
		r[BN_512_SIG] = 0;
} /* end */

/* @func: bn_int512_or - big number or operation
* @param1: bn_int512_t       # result
* @param2: const bn_int512_t # number
* @param3: const bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int512_or)(bn_int512_t r, bn_int512_t a, bn_int512_t b) {
	FSYMBOL(bn_uint512_or)(r, a, b);

	r[BN_512_SIG] = a[BN_512_SIG] || b[BN_512_SIG];
	if (!FSYMBOL(bn_uint512_cmp_1)(r, 0))
		r[BN_512_SIG] = 0;
} /* end */

/* @func: bn_int512_xor - big number xor operation
* @param1: bn_int512_t       # result
* @param2: const bn_int512_t # number
* @param3: const bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int512_xor)(bn_int512_t r, bn_int512_t a, bn_int512_t b) {
	FSYMBOL(bn_uint512_xor)(r, a, b);

	r[BN_512_SIG] = a[BN_512_SIG] ^ b[BN_512_SIG];
	if (!FSYMBOL(bn_uint512_cmp_1)(r, 0))
		r[BN_512_SIG] = 0;
} /* end */

/* @func: bn_int512_not - big number not operation
* @param1: bn_int512_t       # result
* @param2: const bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int512_not)(bn_int512_t r, bn_int512_t n) {
	FSYMBOL(bn_uint512_not)(r, n);

	r[BN_512_SIG] = !n[BN_512_SIG];
	if (!FSYMBOL(bn_uint512_cmp_1)(r, 0))
		r[BN_512_SIG] = 0;
} /* end */

/* @func: bn_int512_add - big number addition
* @param1: bn_int512_t       # sum
* @param2: const bn_int512_t # addend
* @param3: const bn_int512_t # addend
* @return: void
*/
void FSYMBOL(bn_int512_add)(bn_int512_t r, const bn_int512_t a,
		const bn_int512_t b) {
	if (a[BN_512_SIG] == b[BN_512_SIG]) {
		r[BN_512_SIG] = a[BN_512_SIG];
		FSYMBOL(bn_uint512_add)(r, a, b);
	} else {
		int32 k = FSYMBOL(bn_uint512_cmp)(a, b);
		if (k > 0) {
			r[BN_512_SIG] = a[BN_512_SIG];
			FSYMBOL(bn_uint512_sub)(r, a, b);
		} else if (k < 0) {
			r[BN_512_SIG] = b[BN_512_SIG];
			FSYMBOL(bn_uint512_sub)(r, b, a);
		} else {
			FSYMBOL(bn_int512_zero)(r);
		}
	}
} /* end */

/* @func: bn_int512_sub - big number subtraction
* @param1: bn_int512_t       # difference
* @param2: const bn_int512_t # minuend
* @param3: const bn_int512_t # subtract
* @return: void
*/
void FSYMBOL(bn_int512_sub)(bn_int512_t r, const bn_int512_t a,
		const bn_int512_t b) {
	bn_int512_t neg;
	FSYMBOL(bn_int512_move)(neg, b);
	neg[BN_512_SIG] = !neg[BN_512_SIG];

	FSYMBOL(bn_int512_add)(r, a, neg);
} /* end */

/* @func: bn_int512_mul - big number multiplication
* @param1: bn_int512_t       # product
* @param3: const bn_int512_t # multiplicand
* @param2: const bn_int512_t # multiplier
* @return:
*/
void FSYMBOL(bn_int512_mul)(bn_int512_t r, const bn_int512_t a,
		const bn_int512_t b) {
	FSYMBOL(bn_uint512_mul)(r, a, b);
	r[BN_512_SIG] = a[BN_512_SIG] ^ b[BN_512_SIG];

	if (!FSYMBOL(bn_int512_cmp_1)(r, 0))
		r[BN_512_SIG] = 0;
} /* end */

/* @func: bn_int512_div - big number division
* @param1: bn_int512_t       # quotient
* @param2: bn_int512_t       # remainder
* @param3: const bn_int512_t # dividend
* @param4: const bn_int512_t # divisor
* @return: void
*/
void FSYMBOL(bn_int512_div)(bn_int512_t quo, bn_int512_t rem,
		const bn_int512_t a, const bn_int512_t b) {
	FSYMBOL(bn_uint512_div)(quo, rem, a, b);
	quo[BN_512_SIG] = a[BN_512_SIG] ^ b[BN_512_SIG];
	rem[BN_512_SIG] = a[BN_512_SIG];

	if (!FSYMBOL(bn_uint512_cmp_1)(quo, 0))
		quo[BN_512_SIG] = 0;
	if (!FSYMBOL(bn_uint512_cmp_1)(rem, 0))
		rem[BN_512_SIG] = 0;
} /* end */

/* @func: bn_int512_divmod - big number modulus (mathematical definition)
* @param1: bn_int512_t       # quotient
* @param2: bn_int512_t       # remainder
* @param3: const bn_int512_t # dividend
* @param4: const bn_int512_t # divisor
* @return: void
*/
void FSYMBOL(bn_int512_divmod)(bn_int512_t quo, bn_int512_t rem,
		const bn_int512_t a, const bn_int512_t b) {
	FSYMBOL(bn_int512_div)(quo, rem, a, b);

	if (rem[BN_512_SIG]) {
		FSYMBOL(bn_int512_add)(rem, rem, b);
		rem[BN_512_SIG] = 0;
	}
} /* end */

/* @func: bn_int512_strtonum - string convert big number
* @param1: bn_int512_t # number
* @param2: char **     # end pointer
* @param3: int32       # base type
* @return: void
*/
void FSYMBOL(bn_int512_strtonum)(bn_int512_t t, const char *s, char **e,
		int32 b) {
	for (; *s == ' '; s++);

	int32 neg = 0;
	if (*s == '-' || *s == '+')
		neg = (*s++ == '-') ? 1 : 0;

	FSYMBOL(bn_uint512_strtonum)(t, s, e, b);
	t[BN_512_SIG] = neg;
} /* end */

/* @func: bn_int512_numtostr - big number to string
* @param1: char *            # string buffer
* @param2: const bn_int512_t # number
* @return: void
*/
void FSYMBOL(bn_int512_numtostr)(char *buf, const bn_int512_t n) {
	if (n[BN_512_SIG])
		*buf++ = '-';
	
	FSYMBOL(bn_uint512_numtostr)(buf, n);
} /* end */
